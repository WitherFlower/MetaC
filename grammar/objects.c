#include "objects.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

/// Utils

int   Buffer_length (Buffer *buf)       { return buf->length;       }
void  Buffer_reset(Buffer *buf)         {        buf->length = 0; }

Buffer *Buffer_append(Buffer *buf, int c) {
    if (buf->length >= buf->capacity) {
        buf->capacity = buf->capacity ? buf->capacity * 2 : 32;
        buf->content  = realloc(buf->content, buf->capacity);
    }
    buf->content[buf->length++] = c;
    return buf;
}

Buffer *Buffer_appendAll(Buffer *buf, char *s) {
    while (*s) Buffer_append(buf, *s++);
    return buf;
}

Buffer *Buffer_format(Buffer *buf, char *fmt, ...) {
    size_t len = 0;                     assert(buf->length <= buf->capacity);
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
    buf->length += len;                 assert(buf->length <= buf->capacity);
    return buf;
}

char *Buffer_content(Buffer *buf) {
    Buffer_append(buf, 0);
    buf->length--;
    return buf->content;
}

/// Objects

const char *const typeNames[] = {
    "Undefined", "Integer", "Float", "String", "Primitive", "Symbol", "Object"
};

union object _nil = { Undefined };

const char *getTypeName(oop obj) {
    int type = getType(obj);            assert(0 <= type && type <= indexableSize(typeNames));
    return typeNames[type];
}

oop _checkType(oop obj, enum Type type, char *file, int line) {
    if (getType(obj) != type) fatal("%s:%d: expected type %d, got %d\n", file, line, type, getType(obj));
    return obj;
}

enum Type getType(oop obj) {
    return obj->type;
}

int is(enum Type type, oop obj)    { return type == getType(obj); }

oop make_(size_t size, enum Type type) {
    oop obj = xmalloc(size);
    obj->type = type;
    return obj;
}

oop newInteger(long value) {
    oop obj = make(Integer);
    _set(obj, Integer, _value, value);
    return obj;
}

long _integerValue(oop obj) {
    return _get(obj, Integer, _value);
}

long integerValue(oop obj, char *op) {
    if (!isInteger(obj)) fatal("%s: non-integer operand", op);
    return _integerValue(obj);
}

oop newFloat(double value) {
    oop obj = make(Float);
    _set(obj, Float, _value, value);
    return obj;
}

double _floatValue(oop obj) {
    return _get(obj, Float, _value);
}

double floatValue(oop obj, char *op) {
    switch (getType(obj)) {
        case Integer:     return (double)_integerValue(obj);
        case Float:   return (double)_floatValue(obj);
        default:        fatal("%s: non-numeric operand", op);
    }
    return 0;
}

oop newStringLen(char *value, int length) {
    oop   obj = make(String);
    char *str = xmallocAtomic(length+1);
    memcpy(str, value, length);
    str[length] = 0;
    _set(obj, String, length, length);
    _set(obj, String, value,  str);
    return obj;
}

oop newString(char *value) {
    return newStringLen(value, strlen(value));
}

int digitValue(int digit, int base) {
    if      ('a' <= digit && digit <= 'z') digit -= 'a' - 10;
    else if ('A' <= digit && digit <= 'Z') digit -= 'A' - 10;
    else if (digit < '0' || digit > '9') return -1;
    return (digit < base) ? digit : -1;
}

int readCharValue(char **stringp, int base, int limit) {
    char *string = *stringp;

    int value = 0;
    int d = 0;

    while (limit-- && *string && (d = digitValue(*string, base)) >= 0) {
        ++string;
        value = value * base + d;
    }

    *stringp = string;
    return value;
}

oop newStringEscaped(char *string) {

    static Buffer buf = Buffer_EMPTY;
    Buffer_reset(&buf);

    while (*string) {
        int c = *string++;

        if ('\\' == c && *string) {
            c = *string++;            assert(c != 0);

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
                case 'x' : c = readCharValue(&string, 16, -1);          break;
                case '0' : c = readCharValue(&string, 8, 3);            break;
                default  : fatal("illegal character escape sequence");  break;
            }
        }
        Buffer_append(&buf, c);
    }
    return newStringLen(Buffer_content(&buf), Buffer_length(&buf));
}

oop newPrimitive(prim_t function) {
    oop obj = make(Primitive);
    _set(obj, Primitive, name, 0);
    _set(obj, Primitive, fixed, 0);
    _set(obj, Primitive, function, function);
    return obj;
}

oop newSymbol(char *name) {
    oop obj = make(Symbol);
    _set(obj, Symbol, name,  strdup(name));
    _set(obj, Symbol, value, nil);
    return obj;
}

char *stringValue(oop obj, char *who) {
    int type = getType(obj);
    if (type == String) return _get(obj, String, value);
    if (type == Symbol) return _get(obj, Symbol, name);
    fatal("%s: non-string operand", who);
    return 0;
}

#define declareProp(NAME)    oop prop_##NAME = 0;
doProperties(declareProp);
#undef declareProp

#define declareSym(NAME)    oop sym_##NAME = 0;
doSymbols(declareSym);
#undef declareSym

oop    *symbols = 0;
size_t nsymbols = 0;

oop intern(char *name) {

    ssize_t lo = 0;
    ssize_t hi = nsymbols - 1;

    while (lo <= hi) {
        ssize_t mid = (lo + hi) / 2;
        oop sym = symbols[mid];
        int cmp = strcmp(name, _get(sym, Symbol, name));
        if      (cmp < 0) hi = mid - 1;
        else if (cmp > 0) lo = mid + 1;
        else              return sym;
    }

    symbols = xrealloc(symbols, sizeof(oop) * ++nsymbols);
    memmove(symbols + lo + 1, symbols + lo, sizeof(oop) * (nsymbols - 1 - lo));
    return symbols[lo] = newSymbol(name);
}

oop new(oop delegate) {
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
        int psize, Property *properties
) {
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
    size_t size = get(obj, Object, isize);
    if (index >= size) fatal("index %zd out of range (%zd)", index, size);
    return _get(obj, Object, indexed)[index];
}

oop Object_atPut(oop obj, size_t index, oop val) {
    size_t size = get(obj, Object, isize);
    if (index >= size) fatal("index %zd out of range (%zd)", index, size);
    return _get(obj, Object, indexed)[index] = val;
}

oop Object_push(oop obj, oop val) {
    size_t  size =  get(obj, Object, isize);
    size_t   cap = _get(obj, Object, icap);
    oop *indexed = _get(obj, Object, indexed);

    if (size >= cap) {
        cap = cap ? cap * 2 : 4;
        indexed = xrealloc(indexed, sizeof(oop) * cap);
        _set(obj, Object, icap,    cap);
        _set(obj, Object, indexed, indexed);
    }

    indexed[size++] = val;
    _set(obj, Object, isize, size);

    return val;
}

oop Object_pop(oop obj) {
    size_t  size =  get(obj, Object, isize);
    if (!size) fatal("pop: object is empty");

    oop *indexed = _get(obj, Object, indexed);
    oop  result  = indexed[--size];
    _set(obj, Object, isize, size);

    return result;
}

oop cloneEmpty(oop obj) {
    return new(get(obj, Object, delegate));
}

ssize_t Object_find(oop obj, oop key) {
    ssize_t lo = 0, hi =  get(obj, Object, psize) - 1;    // asserts obj is Object
    Property *kvs = _get(obj, Object, properties);

    while (lo <= hi) {
        ssize_t mid = (lo + hi) / 2;
        oop  midkey = kvs[mid].key;
        if      (key < midkey) hi = mid - 1;
        else if (key > midkey) lo = mid + 1;
        else                   return mid;
    }
    return -1 - lo;
}

int Object_includes(oop obj, oop key) {
    return Object_find(obj, key) >= 0;
}

oop Object_getLocal(oop obj, oop key) {
    ssize_t ind = Object_find(obj, key);
    if (ind >= 0) return _get(obj, Object, properties)[ind].val;
    return nil;
}

oop Object_get(oop obj, oop key) {
    while (is(Object, obj)) {
        ssize_t ind = Object_find(obj, key);
        if (ind >= 0) return _get(obj, Object, properties)[ind].val;
        obj = _get(obj, Object, delegate);
    }
    return nil;
}

oop lookup(oop obj, oop key) {
    while (is(Object, obj)) {
        ssize_t ind = Object_find(obj, key);
        if (ind >= 0) return _get(obj, Object, properties)[ind].val;
        obj = _get(obj, Object, delegate);
    }
    return is(Symbol, key) ? _get(key, Symbol, value) : nil;
}

oop setvar(oop obj, oop key, oop val) {
    while (is(Object, obj)) {
        ssize_t ind = Object_find(obj, key);

        if (ind >= 0) return _get(obj, Object, properties)[ind].val = val;

        obj = _get(obj, Object, delegate);
    }
    return is(Symbol, key) ? _set(key, Symbol, value, val) : nil;
}

oop Object_put(oop obj, oop key, oop val) {
    ssize_t ind = Object_find(obj, key);
    Property *kvs = _get(obj, Object, properties);

    if (ind < 0) {
        int size = _get(obj, Object, psize);
        ind = -1 - ind;
        assert(0 <= ind && ind <= size);
        kvs = xrealloc(kvs, sizeof(*kvs) * ++size);
        _set(obj, Object, properties, kvs);
        _set(obj, Object, psize,      size);
        memmove(kvs + ind + 1, kvs + ind, sizeof(*kvs) * (size - 1 - ind));
        kvs[ind].key = key;
    }

    assert(ind < _get(obj, Object, psize));
    assert(kvs[ind].key == key);
    return kvs[ind].val = val;
}

Buffer *printOn(Buffer *buf, oop obj, int indent) {
    // printf("%u\n", obj->type);
    switch (getType(obj)) {
        case Undefined: Buffer_appendAll(buf, "nil");                                                           break;
        case Integer:   Buffer_format(buf, "%ld", _integerValue(obj));                                          break;
        case Float:     Buffer_format(buf, "%f" , _floatValue(obj));                                            break;
        case String:    Buffer_format(buf, "%.*s", (int)_get(obj, String, length), _get(obj, String, value));   break;

        case Primitive: {
            Buffer_appendAll(buf, "<primitive ");
            if (_get(obj, Primitive, name)) printOn(buf, _get(obj, Primitive, name), indent);
            else                           Buffer_format(buf, "%p", _get(obj, Primitive, function));
            Buffer_append(buf, '>');
            break;
        }

        case Symbol:
            Buffer_appendAll(buf, _get(obj, Symbol, name));
            break;

        case Object: {
            int level = 1;
            oop proto = obj;
            oop name  = nil;

            do {
                name = Object_getLocal(proto, prop_name);
                if (nil != name) break;
                ++level;
                proto = _get(proto, Object, delegate);
            } while (is(Object, proto));

            for (int i = level;  i--;)
                Buffer_append(buf, '<');

            if (name != nil)
                printOn(buf, name, indent);
            else
                Buffer_appendAll(buf, "<?>");

            for (int i = level;  i--;)
                Buffer_append(buf, '>');

            if (!indent) break;

            for (;;) {

                int psize = _get(obj, Object, psize);
                Property *props = _get(obj, Object, properties);
                for (int i = 0; i < psize;  ++i) {
                    Buffer_append(buf, '\n');
                    for (int j = indent;  j--;) Buffer_appendAll(buf, "  | ");
                    Buffer_appendAll(buf, "  ");
                    printOn(buf, props[i].key, indent+1);
                    Buffer_appendAll(buf, ": ");
                    printOn(buf, props[i].val, indent+1);
                }

                int isize = _get(obj, Object, isize);
                oop *indexed = _get(obj, Object, indexed);
                for (int i = 0; i < isize;  ++i) {
                    Buffer_append(buf, '\n');
                    for (int j = indent;  j--;) Buffer_appendAll(buf, "  | ");
                    Buffer_format(buf, "  %d: ", i);
                    printOn(buf, indexed[i], indent+1);
                }

                oop delegate = _get(obj, Object, delegate);
                if (nil == delegate) break;
                if (nil != Object_getLocal(delegate, prop_name)) break;
                obj = delegate;
                // ++indent;
                Buffer_appendAll(buf, " =>");
            }
            break;
    }
        default:
            printf("\n--Indent: %d--\n%s\n\n----\n\n", indent, Buffer_content(buf));
            assert(!"this cannot happen");
    }
    return buf;
}

Buffer *storeOn(Buffer *buf, oop obj, int indent) {
    switch (getType(obj)) {
        case String: {
            Buffer_append(buf, '"');
            char *str = _get(obj, String, value);
            int   len = _get(obj, String, length);
            for (int i = 0;  i < len;  ++i) {
                int c = str[i];
                switch (c) {
                    case '\a': Buffer_appendAll(buf,  "\\a");    break;
                    case '\b': Buffer_appendAll(buf,  "\\b");    break;
                    case '\f': Buffer_appendAll(buf,  "\\f");    break;
                    case '\n': Buffer_appendAll(buf,  "\\n");    break;
                    case '\r': Buffer_appendAll(buf,  "\\r");    break;
                    case '\t': Buffer_appendAll(buf,  "\\t");    break;
                    case '\v': Buffer_appendAll(buf,  "\\v");    break;
                    case  '"': Buffer_appendAll(buf, "\\\"");    break;
                    case '\\': Buffer_appendAll(buf, "\\\\");    break;
                    default:
                        if (c < ' ' || c > '~') Buffer_format(buf, "\\%04o", c);
                        else                    Buffer_append(buf, c);
                        break;
                }
            }
            Buffer_append(buf, '"');
            break;
        }
        default:
            printOn(buf, obj, indent);
    }
    return buf;
}

char *printString(oop obj, int indent) {
    static Buffer buf = Buffer_EMPTY;
    Buffer_reset(&buf);
    printOn(&buf, obj, indent);
    return Buffer_content(&buf);
}

void print(oop obj, int indent) {
    printf("%s", printString(obj, indent));
}

void println(oop obj, int indent) {
    print(obj, indent);
    printf("\n");
}

char *storeString(oop obj, int indent) {
    static Buffer buf = Buffer_EMPTY;
    Buffer_reset(&buf);
    storeOn(&buf, obj, indent);
    return Buffer_content(&buf);
}

void store(oop obj, int indent) {
    printf("%s", storeString(obj, indent));
}

void storeln(oop obj, int indent) {
    store(obj, indent);
    printf("\n");
}

oop trace = nil;

void fatal(char *fmt, ...) {
    fflush(stdout);
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "\n");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    if (is(Object, trace)) {
        for (int i = _get(trace, Object, isize);  i--;) {
            printf("frame %d ----------------\n", i);
            println(_get(trace, Object, indexed)[i], 1);
        }
    }
    exit(1);
}

#include <signal.h>

void sigint(int sig) {
    fatal("keyboard interrupt");
}

/// Prototypes

#define  declareProto(NAME)    oop p##NAME = 0;
doProtos(declareProto)
#undef   declareProto

oop newGrammar() {
    oop o = new(pGrammar);
    return o;
}

oop newDefinition(oop name, oop rule)
{
    oop o = new(pDefinition);
    Object_put(o, sym_name, name);
    Object_put(o, sym_rule, rule);
    return o;
}

oop newAssignment(oop variableName, oop ruleIdentifier) {
    oop o = new(pAssignment);
    Object_put(o, sym_variableName, variableName);
    Object_put(o, sym_ruleIdentifier, ruleIdentifier);
    return o;
}

oop newSequence(oop leftExpression, oop rightExpression) {
    oop o = new(pSequence);
    Object_put(o, sym_leftExpression, leftExpression);
    Object_put(o, sym_rightExpression, rightExpression);
    assert(o->type < 100);
    return o;
}

oop newAlternation(oop leftExpression, oop rightExpression) {
    oop o = new(pAlternation);
    Object_put(o, sym_leftExpression, leftExpression);
    Object_put(o, sym_rightExpression, rightExpression);
    return o;
}

oop newStar(oop expression) {
    oop o = new(pStar);
    Object_put(o, sym_expression, expression);
    return o;
}

oop newPlus(oop expression) {
    oop o = new(pPlus);
    Object_put(o, sym_expression, expression);
    return o;
}

oop newOptional(oop expression) {
    oop o = new(pOptional);
    Object_put(o, sym_expression, expression);
    return o;
}

oop newAnd(oop expression) {
    oop o = new(pAnd);
    Object_put(o, sym_expression, expression);
    return o;
}

oop newNot(oop expression) {
    oop o = new(pNot);
    Object_put(o, sym_expression, expression);
    return o;
}

oop newDot() {
    oop o = new(pDot);
    return o;
}

oop newBegin() {
    oop o = new(pBegin);
    return o;
}

oop newEnd() {
    oop o = new(pEnd);
    return o;
}

oop newCharacterClass(char *value) {
    oop string = newString(value);
    oop o = new(pCharacterClass);
    Object_put(o, sym_value, string);
    return o;
}

oop newAction(oop body) {
    oop o = new(pAction);
    Object_put(o, sym_body, body);
    return o;
}

oop newIdentifier(char *value) {
    oop string = newString(value);
    oop o = new(pIdentifier);
    Object_put(o, sym_value, string);
    return o;
}

/// Metalanguage objects constuctors

oop newGetVar(oop name) {
    oop o = new(pGetVar);
    Object_put(o, sym_name, name);
    return o;
}

oop newSetVar(oop name, oop expr) {
    oop o = new(pSetVar);
    Object_put(o, sym_name, name);
    Object_put(o, sym_expr, expr);
    return o;
}

oop newCall(oop function, oop arguments) {
    oop o = new(pCall);
    Object_put(o, sym_function , function );
    Object_put(o, sym_arguments, arguments);
    return o;
}

oop newBlock(oop body) {
    oop o = new(pBlock);
    Object_put(o, sym_body, body);
    return o;
}

oop newReturnValue() {
    oop o = new(pReturnValue);
    return o;
}

// Function definitions

oop prim_print(oop func, oop self, oop args, oop env) {
    int argc     = _get(args, Object,isize);
    oop *indexed = _get(args, Object,indexed);
    oop result   = nil;
    int indent   = 0;
    if (nil != Object_get(args, sym_full)) indent = 1;
    for (int i = 0;  i < argc;  ++i) print(result = indexed[i], indent);
    return nil;
}

void printTree(oop grammar) {
    oop printFunc = _get(intern("print"), Symbol, value);
    oop args = new(pObject);
    Object_put(args, sym_full, newInteger(1));
    Object_push(args, grammar);
	_get(printFunc, Primitive, function)(printFunc, nil, args, nil);
    printf("\n");
}

void initPrototypeSystem() {
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
    				_set(intern(#NAME), Symbol, value,    p##NAME);	\
    				_set(intern(#NAME), Symbol, prototype, t##NAME);
    doProtos(defineProto);
# undef defineProto

    _set(intern("print"), Symbol,value, newPrimitive(prim_print));
}

// void *getProperty(oop object, char *propertyName) {
//     return get(object->properties, propertyName);
// }
//
// void setProperty(oop object, char *propertyName, void *value) {
//     set(object->properties, propertyName, value);
// }
//
// // Return type of returned function (*name of HOF (args of HOF)) (args of returned function)
// void *(*getMethod(oop object, char *methodName)) (oop, ...) {
//     return get(object->methods, methodName);
// }

