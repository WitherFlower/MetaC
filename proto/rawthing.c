// # minproto.leg -- minimal prototype langauge for semantic experiments
// #
// # last edited: 2024-04-20 07:54:05 by piumarta on m1mbp
//
// %{
// ;
#ifndef GC
# define GC 1
#endif

#ifndef TAGS
# define TAGS 1
#endif

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/errno.h>
#include <sys/resource.h>

#if GC
# include <gc.h>
# define xmalloc(N)		(GC_malloc(N))
# define xmallocAtomic(N)	(GC_malloc_atomic(N))
# define xrealloc(P, N)		(GC_realloc(P, N))
#else
# define GC_INIT()
# define xmalloc(N)		(calloc(1, N))
# define xmallocAtomic(N)	(calloc(1, N))
# define xrealloc(P, N)		(realloc(P, N))
#endif

#define indexableSize(A)	(sizeof(A) / sizeof(*(A)))

void fatal(char *fmt, ...);

int opt_O = 0;
int opt_v = 0;

typedef struct Buffer
{
    char *content;
    int   length;
    int   capacity;
} Buffer;

#define Buffer_EMPTY	{ 0, 0, 0 }

int   Buffer_length (Buffer *buf)	{ return buf->length; 	  }
void  Buffer_reset(Buffer *buf)		{        buf->length = 0; }

Buffer *Buffer_append(Buffer *buf, int c)
{
    if (buf->length >= buf->capacity) {
	buf->capacity = buf->capacity ? buf->capacity * 2 : 32;
	buf->content  = realloc(buf->content, buf->capacity);
    }
    buf->content[buf->length++] = c;
    return buf;
}

Buffer *Buffer_appendAll(Buffer *buf, char *s)
{
    while (*s) Buffer_append(buf, *s++);
    return buf;
}

Buffer *Buffer_format(Buffer *buf, char *fmt, ...)
{
    size_t len = 0;					assert(buf->length <= buf->capacity);
    for (;;) {
	size_t cap = buf->capacity - buf->length;
	va_list ap;
	va_start(ap, fmt);
	len = vsnprintf(buf->content + buf->length, cap, fmt, ap);
	va_end(ap);
	if (len < cap) break;
	buf->capacity += len + 32;
	buf->content   = realloc(buf->content, buf->capacity);
    }
    buf->length += len;					assert(buf->length <= buf->capacity);
    return buf;
}

char *Buffer_content(Buffer *buf)
{
    Buffer_append(buf, 0);
    buf->length--;
    return buf->content;
}

union object;
typedef union object *oop;

#define nil (&_nil)

enum  type {
     Undefined ,  Integer ,  Float ,  String ,  Primitive ,  Symbol ,  Object
};

char *typeNames[] = {
    "Undefined", "Integer", "Float", "String", "Primitive", "Symbol", "Object"
};

typedef oop (*prim_t)(oop func, oop self, oop args, oop env);

#if TAGS
# define TAGBITS	2
# define TAGMASK	3
# define TAGINT		Integer // 1
# define TAGFLT		Float   // 2
#endif

#define doProtos(_) _(Object) _(GetVar) _(SetVar) _(GetProp) _(SetProp) _(GetArray) _(SetArray) _(Call) _(Invoke) _(Lambda) _(Closure) _(Binop) _(If) _(While) _(Block)

#define  declareProto(NAME)	oop p##NAME = 0;
doProtos(declareProto)
#undef   declareProto

#define  declareType(NAME)	t##NAME,
enum typecode {
    doProtos(declareType)
};
#undef   declareType

struct property  { oop key, val; };

struct Integer 	 { enum type type;  long _value; };
struct Float   	 { enum type type;  double _value; };
struct String  	 { enum type type;  int length;  char *value; };
struct Symbol  	 { enum type type;  char *name;  oop value;  enum typecode typecode; };
struct Primitive { enum type type;  int fixed;  oop name;  prim_t function; };
struct Object    { enum type type;  int isize, icap, psize;  oop delegate, *indexed;  struct property *properties; };

union object
{
    enum   type      type;
    struct Integer   Integer;
    struct Float     Float;
    struct String    String;
    struct Primitive Primitive;
    struct Symbol    Symbol;
    struct Object    Object;
};

union object _nil = { Undefined };

#define nil (&_nil)

enum type getType(oop obj)
{
# if TAGS
    if ((intptr_t)obj & TAGMASK) return ((intptr_t)obj & TAGMASK);
# endif
    return obj->type;
}

char *getTypeName(oop obj)
{
    int type = getType(obj);			assert(0 <= type && type <= indexableSize(typeNames));
    return typeNames[type];
}

int is(enum type type, oop obj)	{ return type == getType(obj); }

oop _checkType(oop obj, enum type type, char *file, int line)
{
    if (getType(obj) != type) fatal("%s:%d: expected type %d, got %d\n", file, line, type, getType(obj));
    return obj;
}

#define get(OBJ, TYPE,FIELD)		(_checkType(OBJ, TYPE, __FILE__, __LINE__)->TYPE.FIELD)
#define set(OBJ, TYPE,FIELD, VAL)	(_checkType(OBJ, TYPE, __FILE__, __LINE__)->TYPE.FIELD = VAL)

#ifdef NDEBUG
# define _get(OBJ, TYPE,FIELD)		((OBJ)->TYPE.FIELD)
# define _set(OBJ, TYPE,FIELD, VAL)	((OBJ)->TYPE.FIELD = VAL)
#else
# define _get(OBJ, TYPE,FIELD)		get(OBJ, TYPE,FIELD)
# define _set(OBJ, TYPE,FIELD, VAL )	set(OBJ, TYPE,FIELD, VAL)
#endif

#define make(TYPE)	make_(sizeof(struct TYPE), TYPE)

oop make_(size_t size, enum type type)
{
    oop obj = xmalloc(size);
    obj->type = type;
    return obj;
}

oop newInteger(long value)
{
# if TAGS
    return (oop)((intptr_t)value << TAGBITS | TAGINT);
# else
    oop obj = make(Integer);
    _set(obj, Integer,_value, value);
    return obj;
# endif
}

#define isInteger(obj)	is(Integer, obj)

long _integerValue(oop obj)
{
# if TAGS
    return (intptr_t)obj >> TAGBITS;
# else
    return _get(obj, Integer,_value);
# endif
}

long integerValue(oop obj, char *op)
{
    if (!isInteger(obj)) fatal("%s: non-integer operand", op);
    return _integerValue(obj);
}

oop newFloat(double value)
{
# if TAGS
    union { intptr_t ptr;  double dbl; } bits = { .dbl = value };
    return (oop)((bits.ptr & ~TAGMASK) | TAGFLT);
# else
    oop obj = make(Float);
    _set(obj, Float,_value, value);
    return obj;
# endif
}

double _floatValue(oop obj)
{
# if TAGS
    union { intptr_t ptr;  double dbl; } bits = { .ptr = (intptr_t)obj };
    return bits.dbl;
# else
    return _get(obj, Float,_value);
# endif
}

double floatValue(oop obj, char *op)
{
    switch (getType(obj)) {
	case Integer:	return (double)_integerValue(obj);
	case Float:	return (double)_floatValue(obj);
	default:	fatal("%s: non-numeric operand", op);
    }
    return 0;
}

oop newStringLen(char *value, int length)
{
    oop   obj = make(String);
    char *str = xmallocAtomic(length+1);
    memcpy(str, value, length);
    str[length] = 0;
    _set(obj, String,length, length);
    _set(obj, String,value,  str);
    return obj;
}

oop newString(char *value)
{
    return newStringLen(value, strlen(value));
}

int digitValue(int digit, int base)
{
    if      ('a' <= digit && digit <= 'z') digit -= 'a' - 10;
    else if ('A' <= digit && digit <= 'Z') digit -= 'A' - 10;
    else if (digit < '0' || digit > '9') return -1;
    return (digit < base) ? digit : -1;
}

int readCharValue(char **stringp, int base, int limit)
{
    char *string = *stringp;
    int   value = 0, d = 0;
    while (limit-- && *string && (d = digitValue(*string, base)) >= 0) {
	++string;
	value = value * base + d;
    }
    *stringp = string;
    return value;
}

oop newStringEscaped(char *string)
{
    static Buffer buf = Buffer_EMPTY;
    Buffer_reset(&buf);
    while (*string) {
	int c = *string++;
	if ('\\' == c && *string) {
	    c = *string++;			assert(c != 0);
	    switch (c) {
		case '\"': c = '\"';  break;
		case '\'': c = '\'';  break;
		case '\\': c = '\\';  break;
		case 'a' : c = '\a';  break;
		case 'b' : c = '\b';  break;
		case 'f' : c = '\f';  break;
		case 'n' : c = '\n';  break;
		case 'r' : c = '\r';  break;
		case 't' : c = '\t';  break;
		case 'v' : c = '\v';  break;
		case 'X' :
		case 'x' : c = readCharValue(&string, 16, -1);		break;
		case '0' : c = readCharValue(&string, 8, 3);		break;
		default  : fatal("illegal character escape sequence");	break;
	    }
	}
	Buffer_append(&buf, c);
    }
    return newStringLen(Buffer_content(&buf), Buffer_length(&buf));
}

oop newPrimitive(prim_t function)
{
    oop obj = make(Primitive);
    _set(obj, Primitive,name,     0);
    _set(obj, Primitive,fixed,    0);
    _set(obj, Primitive,function, function);
    return obj;
}

oop newSymbol(char *name)
{
    oop obj = make(Symbol);
    _set(obj, Symbol,name,  strdup(name));
    _set(obj, Symbol,value, nil);
    return obj;
}

char *stringValue(oop obj, char *who)
{
    int type = getType(obj);
    if (type == String) return _get(obj, String,value);
    if (type == Symbol) return _get(obj, Symbol,name);
    fatal("%s: non-string operand", who);
    return 0;
}

#define doProperties(_) _(name)

#define declareProp(NAME)	oop prop_##NAME = 0;
doProperties(declareProp);
#undef declareProp

#define doSymbols(_) _(t) _(name) _(expr) _(function) _(arguments) _(object) _(index) _(key) _(value) _(self) _(method) _(parameters) _(body) _(lambda) _(environment) _(operation) _(full) _(condition) _(consequent) _(alternate)

#define declareSym(NAME)	oop sym_##NAME = 0;
doSymbols(declareSym);
#undef declareSym

oop    *symbols = 0;
size_t nsymbols = 0;

oop intern(char *name)
{
    ssize_t lo = 0, hi = nsymbols - 1;
    while (lo <= hi) {
	ssize_t mid = (lo + hi) / 2;
	oop sym = symbols[mid];
	int cmp = strcmp(name, _get(sym, Symbol,name));
	if      (cmp < 0) hi = mid - 1;
	else if (cmp > 0) lo = mid + 1;
	else              return sym;
    }
    symbols = xrealloc(symbols, sizeof(*symbols) * ++nsymbols);
    memmove(symbols + lo + 1, symbols + lo, sizeof(*symbols) * (nsymbols - 1 - lo));
    return symbols[lo] = newSymbol(name);
}

oop new(oop delegate)
{
    oop obj = make(Object);
    _set(obj, Object, delegate,   delegate);
    _set(obj, Object, isize,      0);
    _set(obj, Object, icap,       0);
    _set(obj, Object, psize,      0);
    _set(obj, Object, indexed,    0);
    _set(obj, Object, properties, 0);
    return obj;
}

oop newObjectWithDelegateIndexedProps(
        oop delegate,
        int isize, oop *indexed,
        int psize, struct property *properties)
{
    oop obj = make(Object);
    _set(obj, Object, delegate,   delegate);
    _set(obj, Object, isize,      isize);
    _set(obj, Object, icap,       isize);
    _set(obj, Object, psize,      psize);
    _set(obj, Object, indexed,    indexed);
    _set(obj, Object, properties, properties);
    return obj;
}

oop Object_at(oop obj, size_t index) {
    size_t size = get(obj, Object,isize);
    if (index >= size) fatal("index %zd out of range (%zd)", index, size);
    return _get(obj, Object,indexed)[index];
}

oop Object_atPut(oop obj, size_t index, oop val) {
    size_t size = get(obj, Object,isize);
    if (index >= size) fatal("index %zd out of range (%zd)", index, size);
    return _get(obj, Object,indexed)[index] = val;
}

oop Object_push(oop obj, oop val) {
    size_t  size =  get(obj, Object,isize);
    size_t   cap = _get(obj, Object,icap );
    oop *indexed = _get(obj, Object,indexed);
    if (size >= cap) {
	cap = cap ? cap * 2 : 4;
	indexed = xrealloc(indexed, sizeof(*indexed) * cap);
	_set(obj, Object,icap,    cap);
	_set(obj, Object,indexed, indexed);
    }
    indexed[size++] = val;
    _set(obj, Object,isize, size);
    return val;
}

oop Object_pop(oop obj)
{
    size_t  size =  get(obj, Object,isize);
    if (!size) fatal("pop: object is empty");
    oop *indexed = _get(obj, Object,indexed);
    oop  result  = indexed[--size];
    _set(obj, Object,isize, size);
    return result;
}

oop cloneEmpty(oop obj)
{
    return new(get(obj, Object,delegate));
}

ssize_t Object_find(oop obj, oop key)
{
    ssize_t lo = 0, hi =  get(obj, Object,psize) - 1;	// asserts obj is Object
    struct property *kvs = _get(obj, Object,properties);
    while (lo <= hi) {
	ssize_t mid = (lo + hi) / 2;
	oop  midkey = kvs[mid].key;
	if      (key < midkey) hi = mid - 1;
	else if (key > midkey) lo = mid + 1;
	else                   return mid;
    }
    return -1 - lo;
}

int Object_includes(oop obj, oop key)
{
    return Object_find(obj, key) >= 0;
}

oop Object_getLocal(oop obj, oop key)
{
    ssize_t ind = Object_find(obj, key);
    if (ind >= 0) return _get(obj, Object,properties)[ind].val;
    return nil;
}

oop Object_get(oop obj, oop key)
{
    while (is(Object,obj)) {
	ssize_t ind = Object_find(obj, key);
	if (ind >= 0) return _get(obj, Object,properties)[ind].val;
	obj = _get(obj, Object,delegate);
    }
    return nil;
}

oop lookup(oop obj, oop key)
{
    while (is(Object,obj)) {
	ssize_t ind = Object_find(obj, key);
	if (ind >= 0) return _get(obj, Object,properties)[ind].val;
	obj = _get(obj, Object,delegate);
    }
    return is(Symbol, key) ? _get(key, Symbol,value) : nil;
}

oop setvar(oop obj, oop key, oop val)
{
    while (is(Object,obj)) {
	ssize_t ind = Object_find(obj, key);
	if (ind >= 0) return _get(obj, Object,properties)[ind].val = val;
	obj = _get(obj, Object,delegate);
    }
    return is(Symbol, key) ? _set(key, Symbol,value, val) : nil;
}

oop Object_put(oop obj, oop key, oop val)
{
    ssize_t ind = Object_find(obj, key);
    struct property *kvs = _get(obj, Object,properties);
    if (ind < 0) {
	int size = _get(obj, Object,psize);
	ind = -1 - ind;					assert(0 <= ind && ind <= size);
	kvs = xrealloc(kvs, sizeof(*kvs) * ++size);
	_set(obj, Object,properties, kvs);
	_set(obj, Object,psize,      size);
	memmove(kvs + ind + 1, kvs + ind, sizeof(*kvs) * (size - 1 - ind));
	kvs[ind].key = key;
    }							assert(ind < _get(obj, Object,psize));
    							assert(kvs[ind].key == key);
    return kvs[ind].val = val;
}

Buffer *printOn(Buffer *buf, oop obj, int indent)
{
    switch (getType(obj)) {
	case Undefined:	Buffer_appendAll(buf, "nil");								break;
	case Integer:	Buffer_format(buf, "%ld", _integerValue(obj));						break;
	case Float:	Buffer_format(buf, "%f" , _floatValue(obj));						break;
	case String:	Buffer_format(buf, "%.*s", (int)_get(obj, String,length), _get(obj, String,value));	break;
	case Primitive: {
	    Buffer_appendAll(buf, "<primitive ");
	    if (_get(obj, Primitive,name)) printOn(buf, _get(obj, Primitive,name), indent);
	    else                           Buffer_format(buf, "%p", _get(obj, Primitive,function));
	    Buffer_append(buf, '>');
	    break;
	}
	case Symbol:	Buffer_appendAll(buf, _get(obj, Symbol,name));						break;
	case Object: {
	    int level = 1;
	    oop proto = obj;
	    oop name  = nil;
	    do {
		name = Object_getLocal(proto, prop_name);
		if (nil != name) break;
		++level;
		proto = _get(proto, Object,delegate);
	    } while (is(Object, proto));
	    for (int i = level;  i--;) Buffer_append(buf, '<');
	    if (name != nil)
		printOn(buf, name, indent);
	    else
		Buffer_appendAll(buf, "<?>");
	    for (int i = level;  i--;) Buffer_append(buf, '>');
	    if (!indent) break;
	    for (;;) {
		int psize = _get(obj, Object,psize);
		struct property *props = _get(obj, Object,properties);
		for (int i = 0; i < psize;  ++i) {
		    Buffer_append(buf, '\n');
		    for (int j = indent;  j--;) Buffer_appendAll(buf, "  | ");
		    Buffer_appendAll(buf, "  ");
		    printOn(buf, props[i].key, indent+1);
		    Buffer_appendAll(buf, ": ");
		    printOn(buf, props[i].val, indent+1);
		}
		int isize = _get(obj, Object,isize);
		oop *indexed = _get(obj, Object,indexed);
		for (int i = 0; i < isize;  ++i) {
		    Buffer_append(buf, '\n');
		    for (int j = indent;  j--;) Buffer_appendAll(buf, "  | ");
		    Buffer_format(buf, "  %d: ", i);
		    printOn(buf, indexed[i], indent+1);
		}
		oop delegate = _get(obj, Object,delegate);
		if (nil == delegate) break;
		if (nil != Object_getLocal(delegate, prop_name)) break;
		obj = delegate;
		// ++indent;
		Buffer_appendAll(buf, " =>");
	    }
	    break;
	}
	default:
	    assert(!"this cannot happen");
    }
    return buf;
}

Buffer *storeOn(Buffer *buf, oop obj, int indent)
{
    switch (getType(obj)) {
	case String: {
	    Buffer_append(buf, '"');
	    char *str = _get(obj, String,value);
	    int   len = _get(obj, String,length);
	    for (int i = 0;  i < len;  ++i) {
		int c = str[i];
		switch (c) {
		    case '\a': Buffer_appendAll(buf,  "\\a");	break;
		    case '\b': Buffer_appendAll(buf,  "\\b");	break;
		    case '\f': Buffer_appendAll(buf,  "\\f");	break;
		    case '\n': Buffer_appendAll(buf,  "\\n");	break;
		    case '\r': Buffer_appendAll(buf,  "\\r");	break;
		    case '\t': Buffer_appendAll(buf,  "\\t");	break;
		    case '\v': Buffer_appendAll(buf,  "\\v");	break;
		    case  '"': Buffer_appendAll(buf, "\\\"");	break;
		    case '\\': Buffer_appendAll(buf, "\\\\");	break;
		    default:
			if (c < ' ' || c > '~') Buffer_format(buf, "\\%04o", c);
			else                    Buffer_append(buf, c);
			break;
		}
	    }
	    Buffer_append(buf, '"');
	    break;
	}
	default:	printOn(buf, obj, indent);
    }
    return buf;
}

char *printString(oop obj, int indent)
{
    static Buffer buf = Buffer_EMPTY;
    Buffer_reset(&buf);
    printOn(&buf, obj, indent);
    return Buffer_content(&buf);
}

void print(oop obj, int indent)
{
    printf("%s", printString(obj, indent));
}

void println(oop obj, int indent)
{
    print(obj, indent);
    printf("\n");
}

char *storeString(oop obj, int indent)
{
    static Buffer buf = Buffer_EMPTY;
    Buffer_reset(&buf);
    storeOn(&buf, obj, indent);
    return Buffer_content(&buf);
}

void store(oop obj, int indent)
{
    printf("%s", storeString(obj, indent));
}

void storeln(oop obj, int indent)
{
    store(obj, indent);
    printf("\n");
}

oop trace = nil;

void fatal(char *fmt, ...)
{
    fflush(stdout);
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "\n");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    if (is(Object, trace)) {
	for (int i = _get(trace, Object,isize);  i--;) {
	    printf("frame %d ----------------\n", i);
	    println(_get(trace, Object,indexed)[i], 1);
	}
    }
    exit(1);
}

#include <signal.h>

void sigint(int sig)
{
    fatal("keyboard interrupt");
}

typedef struct Input {
    struct Input *next;
    char	 *text;
    int 	  size;
    int 	  position;
} Input;

Input *newInput(void)
{
    return xmalloc(sizeof(Input));
}

Input *input = 0;

Input *makeInput(void)
{
    return xmalloc(sizeof(Input));
}

#define YYSTYPE oop

#define YY_INPUT(buf, result, max_size)				\
    {								\
	result= (input->position >= input->size)		\
	    ? 0							\
	    : ((*(buf)= input->text[input->position++]), 1);	\
	/* printf("<%c>", *(buf)); */				\
    }

YYSTYPE yysval = 0;

oop newGetVar(oop name)
{
    oop o = new(pGetVar);
    Object_put(o, sym_name, name);
    return o;
}

oop newSetVar(oop name, oop expr)
{
    oop o = new(pSetVar);
    Object_put(o, sym_name, name);
    Object_put(o, sym_expr, expr);
    return o;
}

oop newGetProp(oop object, oop key)
{
    oop o = new(pGetProp);
    Object_put(o, sym_object, object);
    Object_put(o, sym_key   , key   );
    return o;
}

oop newSetProp(oop object, oop key, oop value)
{
    oop o = new(pSetProp);
    Object_put(o, sym_object, object);
    Object_put(o, sym_key   , key   );
    Object_put(o, sym_value , value );
    return o;
}

oop newGetArray(oop object, oop index)
{
    oop o = new(pGetArray);
    Object_put(o, sym_object, object);
    Object_put(o, sym_index , index );
    return o;
}

oop newSetArray(oop object, oop index, oop value)
{
    oop o = new(pSetArray);
    Object_put(o, sym_object, object);
    Object_put(o, sym_index , index );
    Object_put(o, sym_value , value );
    return o;
}

oop newCall(oop function, oop arguments)
{
    oop o = new(pCall);
    Object_put(o, sym_function , function );
    Object_put(o, sym_arguments, arguments);
    return o;
}

oop newInvoke(oop self, oop method, oop arguments)
{
    oop o = new(pInvoke);
    Object_put(o, sym_self     , self     );
    Object_put(o, sym_method   , method   );
    Object_put(o, sym_arguments, arguments);
    return o;
}

oop newLambda(oop parameters, oop body)
{
    oop o = new(pLambda);
    Object_put(o, sym_parameters, parameters);
    Object_put(o, sym_body      , body      );
    return o;
}

oop newClosure(oop lambda, oop environment)
{
    oop o = new(pClosure);
    Object_put(o, sym_lambda     , lambda     );
    Object_put(o, sym_environment, environment);
    return o;
}

enum binop {
    opLogOr,
    opLogAnd,
    opBitOr,
    opBitXor,
    opBitAnd,
    opEq, opNotEq,
    opLess, opLessEq, opGrtr, opGrtrEq,
    opShl, opShr,
    opAdd, opSub,
    opMul, opDiv, opMod
};

oop newBinop(int operation, oop lhs, oop rhs)
{
    oop o = new(pBinop);
    Object_put(o, sym_operation, newInteger(operation));
    Object_push(o, lhs);
    Object_push(o, rhs);
    return o;
}

oop newIf(oop condition, oop consequent, oop alternate)
{
    oop o = new(pIf);
    Object_put(o, sym_condition,  condition );
    Object_put(o, sym_consequent, consequent);
    Object_put(o, sym_alternate,  alternate );
    return o;
}

oop newWhile(oop condition, oop body)
{
    oop o = new(pWhile);
    Object_put(o, sym_condition, condition );
    Object_put(o, sym_body,      body      );
    return o;
}

oop newBlock(oop body)
{
    oop o = new(pBlock);
    Object_put(o, sym_body, body);
    return o;
}

oop neg(oop);

// %}
//
//
// start	= - ( s:stmt				{ yysval = s }
// 	    | !.				{ yysval = 0 }
// 	    | < (!EOL .)* >			{ fatal("syntax error near: %s", yytext) }
// 	    )
//
// stmt	= WHILE LPAREN c:expr RPAREN s:stmt	{ $$ = newWhile(c, s) }
// 	| IF LPAREN c:expr RPAREN s:stmt
// 	  ( ELSE t:stmt				{ $$ = newIf(c, s, t  ) }
// 	  |					{ $$ = newIf(c, s, nil) }
// 	  )
// 	| b:block				{ $$ = newBlock(b) }
// 	| e:expr SEMI				{ $$ = e }
//
// expr	= p:primary
// 	  ( DOT   i:id         ASSIGN e:expr 	{ $$ = newSetProp(p, i, e) }
//           | LBRAK i:expr RBRAK ASSIGN e:expr	{ $$ = newSetArray(p, i, e) }
// 	  )
// 	| i:id ASSIGN e:expr 			{ $$ = newSetVar(i, e) }
// 	| logor
//
// logor	= l:logand ( BARBAR r:logand		{ l = newBinop(opLogOr,  l, r) }
// 		   )*				{ $$ = l }
//
// logand	= l:bitor ( ANDAND r:bitor		{ l = newBinop(opLogAnd, l, r) }
// 		  )*				{ $$ = l }
//
// bitor	= l:bitxor ( OR r:bitxor		{ l = newBinop(opBitOr,  l, r) }
// 		   )*				{ $$ = l }
//
// bitxor	= l:bitand ( XOR r:bitand		{ l = newBinop(opBitXor, l, r) }
// 		   )*				{ $$ = l }
//
// bitand	= l:eq ( AND r:eq			{ l = newBinop(opBitAnd, l, r) }
// 	       )*				{ $$ = l }
//
// eq	= l:ineq ( EQ    r:ineq			{ l = newBinop(opEq,    l, r) }
// 	  	 | NOTEQ r:ineq			{ l = newBinop(opNotEq, l, r) }
// 	  	 )*				{ $$ = l }
//
// ineq	= l:shift ( LESS   r:shift		{ l = newBinop(opLess,   l, r) }
// 	  	  | LESSEQ r:shift		{ l = newBinop(opLessEq, l, r) }
// 	  	  | GRTREQ r:shift		{ l = newBinop(opGrtrEq, l, r) }
// 	  	  | GRTR   r:shift		{ l = newBinop(opGrtr,   l, r) }
// 	  	  )*				{ $$ = l }
//
// shift	= l:sum ( SHL r:sum			{ l = newBinop(opShl, l, r) }
// 	  	| SHR r:sum			{ l = newBinop(opShr, l, r) }
// 	  	)*				{ $$ = l }
//
// sum	= l:prod ( PLUS  r:prod			{ l = newBinop(opAdd, l, r) }
// 	  	 | MINUS r:prod			{ l = newBinop(opSub, l, r) }
// 	  	 )*				{ $$ = l }
//
// prod	= l:postfix ( STAR  r:postfix		{ l = newBinop(opMul, l, r) }
// 	  	    | SLASH r:postfix		{ l = newBinop(opDiv, l, r) }
// 	  	    | PCENT r:postfix		{ l = newBinop(opMod, l, r) }
// 	  	    )*				{ $$ = l }
//
// postfix	= p:primary
//           ( LBRAK e:expr RBRAK !ASSIGN		{ p = newGetArray(p, e) }
// 	  | DOT   i:id a:args  !ASSIGN	        { p = newInvoke(p, i, a) }
//           | DOT   i:id         !ASSIGN		{ p = newGetProp(p, i) }
//           |            a:args  !ASSIGN		{ p = newCall(p, a) }
// 	  )*					{ $$ = p }
//
// args	= LPAREN a:mklist
// 	  ( ( k:id COLON e:expr			{ Object_put(a, k, e) }
// 	    | e:expr				{ Object_push(a, e) }
// 	    )
//             ( COMMA ( k:id COLON e:expr		{ Object_put(a, k, e) }
// 	    	    | e:expr			{ Object_push(a, e) }
// 		    ) )* )? RPAREN		{ $$ = a }
//
// params	= LPAREN p:mklist
// 	  ( i:id				{ Object_push(p, i) }
//             ( COMMA i:id 			{ Object_push(p, i) }
// 	    )* )? RPAREN			{ $$ = p }
//
// mklist	= 					{ $$ = new(pObject) }
//
// primary	= nil | number | string | symbol | var | lambda | subexpr
//
// lambda	= p:params b:block			{ $$ = newLambda(p, b) }
//
// subexpr	= LPAREN e:expr RPAREN			{ $$ = e }
//
// block	= LBRACE b:mklist
// 	    (  e:stmt		 		{ Object_push(b, e) }
// 	    )* RBRACE				{ $$ = b }
//
// nil   	= NIL					{ $$ = nil }
//
// number	= "-" u:unsign				{ $$ = neg(u) }
// 	| "+" n:number				{ $$ = u }
// 	|     u:unsign				{ $$ = u }
//
// unsign	= < DIGIT+ '.' DIGIT* EXP? > -		{ $$ = newFloat(strtod(yytext, 0)) }
// 	| < DIGIT* '.' DIGIT+ EXP? > -		{ $$ = newFloat(strtod(yytext, 0)) }
// 	| "0" [bB] < BIGIT+ > -			{ $$ = newInteger(strtol(yytext, 0,  2)) }
// 	| "0" [xX] < HIGIT+ > -			{ $$ = newInteger(strtol(yytext, 0, 16)) }
// 	| "0" < OIGIT* > -			{ $$ = newInteger(strtol(yytext, 0,  8)) }
// 	| < DIGIT+ > -				{ $$ = newInteger(strtol(yytext, 0, 10)) }
//
// string	= '"' < ( !'"' . )* > '"' -		{ $$ = newStringEscaped(yytext) }
// 	| "'" < ( !"'" . )* > "'" -		{ $$ = newStringEscaped(yytext) }
//
// symbol	= HASH i:id				{ $$ = i }
//
// var	= i:id					{ $$ = newGetVar(i) }
//
// id	= < LETTER ALNUM* > -			{ $$ = intern(yytext) }
//
// BIGIT	= [0-1]
// OIGIT	= [0-7]
// DIGIT	= [0-9]
// HIGIT	= [0-9A-Fa-f]
// LETTER	= [A-Za-z_]
// ALNUM	= LETTER | DIGIT
// SIGN	= [-+]
// EXP	= [eE] SIGN DIGIT+
//
// -	= SPACE*
//
// SPACE	= [ \t\n\r] | '//' (!EOL .)*
// EOL	= [\n\r]
//
// NIL	= "nil"   !ALNUM  -
// WHILE	= "while" !ALNUM  -
// IF	= "if"    !ALNUM  -
// ELSE	= "else"  !ALNUM  -
//
// HASH	= "#"      	-
// SEMI	= ";"      	-
// ASSIGN	= "="  ![=] 	-
// COMMA	= ","      	-
// COLON	= ":"      	-
// LPAREN	= "(" 		-
// RPAREN	= ")" 		-
// LBRAK	= "[" 		-
// RBRAK	= "]" 		-
// LBRACE	= "{" 		-
// RBRACE	= "}" 		-
// BARBAR  = "||" ![=]	-
// ANDAND  = "&&" ![=]	-
// OR	= "|"  ![|=]	-
// XOR	= "^"  ![=]	-
// AND	= "&"  ![&=]	-
// EQ	= "=="	     	-
// NOTEQ	= "!="	     	-
// LESS    = "<"  ![<=]   	-
// LESSEQ  = "<="   	-
// GRTREQ  = ">="   	-
// GRTR    = ">"  ![=] 	-
// SHL	= "<<" ![=]   	-
// SHR	= ">>" ![=]   	-
// PLUS	= "+"  ![+=]   	-
// MINUS	= "-"  ![-=]    -
// STAR	= "*"  ![=]     -
// SLASH	= "/"  ![/=]    -
// PCENT	= "%"  ![*=]    -
// DOT	= "."	     	-
//
// # EQUAL	= "=="       -
// # PLING	= "!"	     -
// # AND	= "&"	     -
// # COLON	= ":"	     -
// # PLUS	= "+"	     -
// # STAR	= "*"	     -
// # QUERY	= "?"	     -
// # BAR	= "|"	     -
// # LANGLE	= "<" -
// # RANGLE	= ">" -
//
// %%;

#define SEND(RCV, MSG) ({						\
	    oop _rcv = RCV;						\
	    oop _fun = Object_get(_rcv, sym_##MSG);			\
	    get(_fun, Primitive,function)(_fun, _rcv, nil, nil);	\
	})

oop sym_x = 0;
oop sym_y = 0;

oop Point_magnitude(oop func, oop self, oop args, oop env)
{
    double x = floatValue(Object_get(self, sym_x), "Point.magnitude");
    double y = floatValue(Object_get(self, sym_y), "Point.magnitude");
    return newFloat(sqrt(x * x + y * y));
}

oop shl(oop l, oop r)
{
    int tl = getType(l), tr = getType(r);
    if (Integer == tl && Integer == tr)
	return newInteger(_integerValue(l) << _integerValue(r));
    fatal("<<: illegal operand types %d and %d", tl, tr);
    return 0;
}

oop shr(oop l, oop r)
{
    int tl = getType(l), tr = getType(r);
    if (Integer == tl && Integer == tr)
	return newInteger(_integerValue(l) >> _integerValue(r));
    fatal(">>: illegal operand types %d and %d", tl, tr);
    return 0;
}

#define binop(NAME, OP)											\
oop NAME(oop l, oop r)											\
{													\
    int tl = getType(l), tr = getType(r);								\
    if (Integer == tl && Integer == tr) return newInteger(_integerValue(l   ) OP _integerValue(r   ));	\
    if (Float   == tl || Float   == tr) return newFloat  ( floatValue(l, #OP) OP  floatValue(r, #OP));	\
    fatal(#OP": illegal operand types %d and %d", tl, tr);						\
    return 0;												\
}

binop(add, +);
binop(sub, -);
binop(mul, *);

#undef binop

oop quo(oop l, oop r)
{
    int tl = getType(l), tr = getType(r);
    if (Integer == tl && Integer == tr) {
	long   vl = _integerValue(l), vr = _integerValue(r);
	ldiv_t qr = ldiv(vl, vr);
	if (!qr.rem) return newInteger(qr.quot);   // division was exact
	return newFloat((double)vl / (double)vr);
    }
    if (Float == tl || Float == tr) return newFloat  (floatValue(l, "/") / floatValue(r, "/"));
    fatal("/: illegal operand types %d and %d", tl, tr);
    return 0;
}

oop rem(oop l, oop r)
{
    int tl = getType(l), tr = getType(r);
    if (Integer == tl && Integer == tr) return newInteger(  _integerValue(l)  % _integerValue(r     ) );
    if (Float   == tl || Float   == tr) return newFloat  (fmod(floatValue(l, "%"), floatValue(r, "%")));
    fatal("/: illegal operand types %d and %d", tl, tr);
    return 0;
}

intptr_t cmp(oop l, oop r, char *who)
{
    int tl = getType(l), tr = getType(r);
    if (Integer == tl && Integer == tr) return _integerValue(l)   - _integerValue(r);
    if (Float   == tl || Float   == tr) return floatValue(l, who) - floatValue(r, who);
    if (String  == tl || String  == tr) return strcmp(stringValue(l, who), stringValue(r, who));
    if (Symbol  == tl || Symbol  == tr) return strcmp(stringValue(l, who), stringValue(r, who));
    return (intptr_t)l - (intptr_t)r;
}

oop neg(oop n)
{
    int tn = getType(n);
    switch (tn) {
	case Integer:	return newInteger(-_integerValue(n));
	case Float:	return newFloat  (-_floatValue  (n));
	default:	break;
    }
    fatal("-: illegal operand type %d", tn);
    return 0;
}

oop evargs(oop list, oop env);

enum typecode getTypecode(oop exp)
{
    oop delegate       = _get(exp, Object,delegate);
    oop name           = Object_get(delegate, prop_name);
    enum typecode type = _get(name, Symbol,typecode);
    return type;
}

oop eval(oop exp, oop env);

oop apply(oop func, oop self, oop args, oop env)
{
    int functype = getType(func);
    if (Primitive == functype)
	return _get(func, Primitive,function)(func, self, args, env);
    if (Object != functype || tClosure  != getTypecode(func))
	fatal("cannot %s %s", (nil == self ? "call" : "invoke"), storeString(func, 0));
    oop  lambda      = Object_get(func,   sym_lambda);
    oop  environment = Object_get(func,   sym_environment);
    oop  parameters  = Object_get(lambda, sym_parameters);
    oop  body        = Object_get(lambda, sym_body);
    oop *exprs       =  get(body, Object,indexed);
    int  size        = _get(body, Object,isize);
    oop  result      = nil;
    _set(args, Object,delegate, environment);      // inherit from closure's captured environment
    Object_put(args, sym_self, self);
    int  nparam = _get(parameters, Object,isize);
    oop *pparam = _get(parameters, Object,indexed);
    int  nargs  = _get(args,       Object,isize);
    oop *pargs  = _get(args,       Object,indexed);
    for (int i = 0;  i < nparam;  ++i)
	Object_put(args, pparam[i], i < nargs ? pargs[i] : nil);
    for (int i = 0;  i < size;  ++i)
	result = eval(exprs[i], args);
    return result;
}

static inline oop evalobj(oop exp, oop env)
{
    enum typecode type = getTypecode(exp);
    switch (type) {
	case tObject:	break;
	case tGetVar: {
	    oop key = Object_get(exp, sym_name);
	    return lookup(env, key);
	}
	case tSetVar: {
	    oop key =      Object_get(exp, sym_name)      ;
	    oop val = eval(Object_get(exp, sym_expr), env);
	    return setvar(env, key, val);
	}
	case tGetProp: {
	    oop obj = eval(Object_get(exp, sym_object), env);
	    oop key =      Object_get(exp, sym_key   )      ;
	    return Object_get(obj, key);
	}
	case tSetProp: {
	    oop obj = eval(Object_get(exp, sym_object), env);
	    oop key =      Object_get(exp, sym_key   )      ;
	    oop val = eval(Object_get(exp, sym_value ), env);
	    return Object_put(obj, key, val);
	}
	case tGetArray: {
	    oop obj = eval(Object_get(exp, sym_object), env);
	    oop ind = eval(Object_get(exp, sym_index ), env);
	    return Object_at(obj, integerValue(ind, "[]"));
	}
	case tSetArray: {
	    oop obj = eval(Object_get(exp, sym_object), env);
	    oop ind = eval(Object_get(exp, sym_index ), env);
	    oop val = eval(Object_get(exp, sym_value ), env);
	    return Object_atPut(obj, integerValue(ind, "[]="), val);
	}
	case tCall: {
	    oop func = eval  (Object_get(exp, sym_function ), env);
	    oop args = evargs(Object_get(exp, sym_arguments), env);
	    return apply(func, nil, args, env);
	}
	case tInvoke: {
	    oop self = eval  (Object_get(exp, sym_self     ), env);
	    oop meth =        Object_get(exp, sym_method   )       ;
	    oop args = evargs(Object_get(exp, sym_arguments), env);
	    oop func = Object_get(self, meth);
	    return apply(func, self, args, env);
	}
	case tLambda: {
	    return newClosure(exp, env);
	}
	case tClosure: break;
	case tBinop: {					assert(_get(exp, Object,isize) == 2);
	    oop op   = Object_get(exp, sym_operation);
	    oop lhs  = _get(exp, Object,indexed)[0];
	    oop rhs  = _get(exp, Object,indexed)[1];
	    enum binop code = integerValue(op, "Binop.operation");
	    lhs = eval(lhs, env);
	    switch (code) {
		case opLogOr:	return nil != lhs ? lhs : eval(rhs, env);
		case opLogAnd:	return nil == lhs ? lhs : eval(rhs, env);
		default:	break;
	    }
	    rhs = eval(rhs, env);
	    switch (code) {
		case opBitOr:	return newInteger(integerValue(lhs, "|") | integerValue(rhs, "|"));
		case opBitXor:	return newInteger(integerValue(lhs, "^") ^ integerValue(rhs, "^"));
		case opBitAnd:	return newInteger(integerValue(lhs, "&") & integerValue(rhs, "&"));
#               define newBoolean(TF)	((TF) ? sym_t : nil)
		case opEq:	return newBoolean(cmp(lhs, rhs, "==") == 0);
		case opNotEq:	return newBoolean(cmp(lhs, rhs, "!=") != 0);
		case opLess:	return newBoolean(cmp(lhs, rhs, "<" ) <  0);
		case opLessEq:	return newBoolean(cmp(lhs, rhs, "<=") <= 0);
		case opGrtrEq:	return newBoolean(cmp(lhs, rhs, ">=") >= 0);
		case opGrtr:	return newBoolean(cmp(lhs, rhs, ">" ) >  0);
#               undef newBoolean
		case opShl:	return shl(lhs, rhs);
		case opShr:	return shr(lhs, rhs);
		case opAdd:  	return add(lhs, rhs);
		case opSub:  	return sub(lhs, rhs);
		case opMul:  	return mul(lhs, rhs);
		case opDiv:  	return quo(lhs, rhs);
		case opMod:  	return rem(lhs, rhs);
		default:     	break;
	    }
	    fatal("illegal binary operation %d", code);
	    return 0;
	}
	case tIf: {
	    oop condition  = eval(Object_get(exp, sym_condition ), env);
	    oop consequent = 	  Object_get(exp, sym_consequent)      ;
	    oop alternate  = 	  Object_get(exp, sym_alternate )      ;
	    return eval(nil != condition ? consequent : alternate, env);
	}
	case tWhile: {
	    oop condition = Object_get(exp, sym_condition);
	    oop body      = Object_get(exp, sym_body     );
	    oop result    = nil;
	    while (nil != eval(condition, env)) result = eval(body, env);
	    return result;
	}
	case tBlock: {
	    oop body     = Object_get(exp, sym_body);
	    oop *indexed = _get(body, Object,indexed);
	    int size     = _get(body, Object,isize);
	    oop result   = nil;
	    for (int i = 0;  i < size;  ++i) result = eval(indexed[i], env);
	    return result;
	}
    }
    return exp;
}

oop eval(oop exp, oop env)
{
    if (Object != getType(exp)) return exp;
    if (!opt_O) Object_push(trace, exp);
    oop result = evalobj(exp, env);
    if (!opt_O) Object_pop(trace);
    return result;
}

#if 0

oop evargs(oop list, oop env)
{
    if (!is(Object, list)) return list;

    oop 	     args    = new(_get(list, Object,delegate));
    oop 	    *indexed = 	   _get(list, Object,indexed);
    int 	     isize   = 	   _get(list, Object,isize);
    int 	     psize   =     _get(list, Object,psize);
    struct property *props   =     _get(list, Object,properties);

    for (int i = 0;  i < isize;  ++i) Object_push(args, eval(indexed[i], env));
    for (int i = 0;  i < psize;  ++i) Object_put(args, props[i].key, props[i].val);
    return args;
}

#else

oop evargs(oop list, oop env)
{
    if (!is(Object, list)) return list;
    int 	     isize    = _get(list, Object,isize);
    int 	     psize    = _get(list, Object,psize);
    oop 	    *indexed  = _get(list, Object,indexed);
    struct property *props    = _get(list, Object,properties);
    oop             *indexed2 = isize ? xmalloc(sizeof(*indexed2) * isize) : 0;
    struct property *props2   = psize ? xmalloc(sizeof(*props2  ) * psize) : 0;
    for (int i = 0;  i < isize;  ++i)
	indexed2[i] = eval(indexed[i], env);
    for (int i = 0;  i < psize;  ++i) {
	props2[i].key =      props[i].key      ;
	props2[i].val = eval(props[i].val, env);
    }
    return newObjectWithDelegateIndexedProps(_get(list, Object,delegate),
					     isize, indexed2,
					     psize, props2);
}

#endif

oop prim_new(oop func, oop self, oop args, oop env)
{							assert(is(Object, args));
    _set(args, Object,delegate, self);
    return args;
}

oop prim_push(oop func, oop self, oop args, oop env)
{							assert(is(Object, args));
    if (!is(Object, self)) fatal("push: not an object");
    int argc     = _get(args, Object,isize);
    oop *indexed = _get(args, Object,indexed);
    for (int i = 0;  i < argc;  ++i) Object_push(self, indexed[i]);
    return self;
}

oop prim_pop(oop func, oop self, oop args, oop env)
{							assert(is(Object, args));
    if (!is(Object, self)) fatal("pop: not an object");
    int size = _get(self, Object,isize);
    if (size < 1) fatal("pop: object is empty\n");
    --size;
    _set(self, Object,isize, size);
    return _get(self, Object,indexed)[size];
}

oop prim_length(oop func, oop self, oop args, oop env)
{							assert(is(Object, args));
    if (!is(Object, self)) fatal("pop: not an object");
    return newInteger(_get(self, Object,isize));
}

oop prim_env(oop func, oop self, oop args, oop env)
{
    return env;
}

oop prim_print(oop func, oop self, oop args, oop env)
{
    int argc     = _get(args, Object,isize);
    oop *indexed = _get(args, Object,indexed);
    oop result   = nil;
    int indent   = 0;
    if (nil != Object_get(args, sym_full)) indent = 1;
    for (int i = 0;  i < argc;  ++i) print(result = indexed[i], indent);
    return nil;
}

oop prim_sqrt(oop func, oop self, oop args, oop env)
{
    int argc = _get(args, Object,isize);
    if (1 != argc) fatal("sqrt: 1 argument expeted");
    return newFloat(sqrt(floatValue(_get(args, Object,indexed)[0], "sqrt")));
}

oop prim_round(oop func, oop self, oop args, oop env)
{
    int argc = _get(args, Object,isize);
    if (1 != argc) fatal("round: 1 argument expeted");
    return newInteger(round(floatValue(_get(args, Object,indexed)[0], "round")));
}

oop prim_truncate(oop func, oop self, oop args, oop env)
{
    int argc = _get(args, Object,isize);
    if (1 != argc) fatal("truncate: 1 argument expeted");
    return newInteger(floatValue(_get(args, Object,indexed)[0], "truncate"));
}

oop prim_cputime(oop func, oop self, oop args, oop env)
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return newFloat(ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1000000.0);
}

void readFile(FILE *file, char **textp, int *sizep)
{
    size_t size = 0;
    char  *text = xmallocAtomic(4096);
    for (;;) {
	ssize_t n = fread(text+size, 1, 4096, file);
	if (n < 1) break;
	size += n;
	if (n < 4096) break;
	text = xrealloc(text, size + 4096);
    }
    *textp = text;
    *sizep = size;
}

oop replFile(FILE *in)
{
    input = newInput();
    readFile(in, &input->text, &input->size);
    oop result = nil;
    while (yyparse() && yysval) {
	if      (opt_v >= 3) storeln(yysval, 1);
	else if (opt_v >= 2) storeln(yysval, 0);
	result = eval(yysval, nil);
	if (opt_v) {
	    printf("  => ");
	    if      (opt_v >= 3) storeln(result, 1);
	    else if (opt_v >= 1) storeln(result, 0);
	}
    }
    return result;
}

oop replPath(char *path)
{
    FILE *in = fopen(path, "r");
    if (!in) fatal("%s: %s", path, strerror(errno));
    oop result = replFile(in);
    fclose(in);
    return result;
}

int main(int argc, char **argv)
{
    GC_INIT();

# define defineProp(NAME) 	prop_##NAME = intern("__"#NAME"__");
    doProperties(defineProp);
# undef defineProp

# define defineSym(NAME) 	sym_##NAME = intern(#NAME);
    doSymbols(defineSym);
# undef defineSym

    pObject = nil;

# define defineProto(NAME)	p##NAME = new(pObject);				\
				Object_put(p##NAME, prop_name, intern(#NAME));	\
    				_set(intern(#NAME), Symbol,value,    p##NAME);	\
    				_set(intern(#NAME), Symbol,typecode, t##NAME);
    doProtos(defineProto);
# undef defineProto

    _set(intern("__env__" ), Symbol,value, newPrimitive(prim_env));
    _set(intern("print"   ), Symbol,value, newPrimitive(prim_print));
    _set(intern("sqrt"    ), Symbol,value, newPrimitive(prim_sqrt));
    _set(intern("round"   ), Symbol,value, newPrimitive(prim_round));
    _set(intern("truncate"), Symbol,value, newPrimitive(prim_truncate));
    _set(intern("cputime" ), Symbol,value, newPrimitive(prim_cputime));

    Object_put(pObject, intern("new"),    newPrimitive(prim_new   ));
    Object_put(pObject, intern("push"),   newPrimitive(prim_push  ));
    Object_put(pObject, intern("pop"),    newPrimitive(prim_pop   ));
    Object_put(pObject, intern("length"), newPrimitive(prim_length));

    trace = new(pObject);

    signal(SIGINT, sigint);

    int repled = 0;

    for (int argn = 1;  argn < argc;  ++argn) {
	char *arg = argv[argn];
	if ('-' == *arg) {
	    while (*++arg) {
		switch (*arg) {
		    case 'O':	++opt_O;	break;
		    case 'v':	++opt_v;	break;
		    default:	fatal("unknown command-line option '%c'", *arg);
		}
	    }
	}
	else {
	    replPath(arg);
	    ++repled;
	}
    }

    if (!repled) replFile(stdin);

    return 0;
}
