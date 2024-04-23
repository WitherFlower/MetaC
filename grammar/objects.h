#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdlib.h>

/// Utils

#ifndef GC
#define GC 1
#endif

#if GC
# include <gc.h>
# define xmalloc(N)        (GC_malloc(N))
# define xmallocAtomic(N)    (GC_malloc_atomic(N))
# define xrealloc(P, N)        (GC_realloc(P, N))
#else
# define GC_INIT()
# define xmalloc(N)        (calloc(1, N))
# define xmallocAtomic(N)    (calloc(1, N))
# define xrealloc(P, N)        (realloc(P, N))
#endif

#define indexableSize(A)    (sizeof(A) / sizeof(*(A)))

void fatal(char *format, ...);

typedef struct Buffer {
    char *content;
    int   length;
    int   capacity;
} Buffer;

#define Buffer_EMPTY    { 0, 0, 0 }

int   Buffer_length (Buffer *buf);
void  Buffer_reset(Buffer *buf);

Buffer *Buffer_append(Buffer *buf, int c);
Buffer *Buffer_appendAll(Buffer *buf, char *s);
Buffer *Buffer_format(Buffer *buf, char *fmt, ...);
char *Buffer_content(Buffer *buf);

/// Objects

union object;
typedef union object *oop;

typedef enum Type {
     Undefined ,  Integer ,  Float ,  String ,  Primitive ,  Symbol ,  Object
} Type;

extern const char *const typeNames[];

typedef oop (*prim_t)(oop func, oop self, oop args, oop env);

typedef struct Property {
    oop key;
    oop val;
} Property;

#define doProtos(_) _(Object) _(Grammar) _(Definition) _(Assignment) _(Sequence) _(Alternation) _(Star) _(Plus) _(Optional) _(And) _(Not) _(Dot) _(Begin) _(End) _(String) _(CharacterClass) _(Action) _(Identifier)

#define declarePrototype(NAME)    t##NAME,
enum Prototype {
    doProtos(declarePrototype)
};
#undef declarePrototype

struct Integer {
    Type type;
    long _value;
};

struct Float {
    Type type;
    double _value;
};

struct String {
    Type type;
    int length;
    char *value;
};

struct Symbol {
    Type type;
    char *name;
    oop value;
    enum Prototype prototype;
};

struct Primitive {
    Type type;
    int fixed;
    oop name;
    prim_t function;
};

struct Object {
    Type type;
    int isize, icap, psize;
    oop delegate, *indexed;
    struct Property *properties;
};

union object {
    Type             type;
    struct Integer     Integer;
    struct Float   Float;
    struct String    String;
    struct Primitive Primitive;
    struct Symbol    Symbol;
    struct Object    Object;
};

extern union object _nil;

#define nil (&_nil)

const char *getTypeName(oop obj);

Type getType(oop obj);

int is(Type type, oop obj);

oop _checkType(oop obj, Type type, char *file, int line);

#define get(OBJ, TYPE,FIELD)		(_checkType(OBJ, TYPE, __FILE__, __LINE__)->TYPE.FIELD)
#define set(OBJ, TYPE,FIELD, VAL)	(_checkType(OBJ, TYPE, __FILE__, __LINE__)->TYPE.FIELD = VAL)

#ifdef NDEBUG
# define _get(OBJ, TYPE,FIELD)		((OBJ)->TYPE.FIELD)
# define _set(OBJ, TYPE,FIELD, VAL)	((OBJ)->TYPE.FIELD = VAL)
#else
# define _get(OBJ, TYPE,FIELD)      get(OBJ, TYPE,FIELD)
# define _set(OBJ, TYPE,FIELD, VAL) set(OBJ, TYPE,FIELD, VAL)
#endif

oop make_(size_t size, Type type);

#define make(TYPE)    make_(sizeof(struct TYPE), TYPE)

oop newInteger(long value);
long integerValue(oop obj, char *op);

#define isInteger(obj)  is(Integer, obj)

oop newFloat(double value);
double floatValue(oop obj, char *op);

oop newString(char *value);
int digitValue(int digit, int base);
int readCharValue(char **stringp, int base, int limit);
oop newStringEscaped(char *string);

oop newPrimitive(prim_t function);

oop newSymbol(char *name);
char *stringValue(oop obj, char *who);

#define doProperties(_) _(name)

#define declareProp(NAME)    extern oop prop_##NAME;
doProperties(declareProp);
#undef declareProp

#define  declareProto(NAME)	extern oop p##NAME;
doProtos(declareProto)
#undef   declareProto

#define doSymbols(_) _(t) _(name) _(rule) _(variableName) _(ruleIdentifier) _(leftExpression) _(rightExpression) _(expression) \
        _(expr) _(function) _(arguments) _(object) _(index) _(key) _(value) _(self) _(method) _(parameters) _(body) _(lambda) _(environment) _(operation) _(full) _(condition) _(consequent) _(alternate)

#define declareSym(NAME)    extern oop sym_##NAME;
doSymbols(declareSym);
#undef declareSym

oop intern(char *name);

oop new(oop delegate);
oop newObjectWithDelegateIndexedProps(
        oop delegate,
        int isize, oop *indexed,
        int psize, Property *properties
);

oop Object_at(oop obj, size_t index);
oop Object_atPut(oop obj, size_t index, oop val);
oop Object_push(oop obj, oop val);
oop Object_pop(oop obj);

oop cloneEmpty(oop obj);

ssize_t Object_find(oop obj, oop key);
int Object_includes(oop obj, oop key);
oop Object_getLocal(oop obj, oop key);
oop Object_get(oop obj, oop key);
oop Object_put(oop obj, oop key, oop val);

void println(oop obj, int indent);

/// Prototypes

// oop lookup(oop obj, oop key);
// oop setvar(oop obj, oop key, oop val);

oop newGrammar();

oop newDefinition(oop name, oop rule);

oop newAssignment(oop variableName, oop ruleIdentifier);

oop newSequence(oop leftExpression, oop rightExpression);

oop newAlternation(oop leftExpression, oop rightExpression);

oop newStar(oop expression);

oop newPlus(oop expression);

oop newOptional(oop expression);

oop newAnd(oop expression);

oop newNot(oop expression);

oop newDot();

oop newBegin();

oop newEnd();

oop newCharacterClass(char *value);

oop newAction(char *value, oop body);

oop newIdentifier(char *value);

// Function definitions

oop prim_print(oop func, oop self, oop args, oop env);

void printTree(oop grammar);

void initPrototypeSystem();

// int declareType(char *name, Dict *methods);
//
// struct Object {
//     char *type;
//     Dict *properties;
//     Dict *methods;
// };
//
// oop newObject(char *typeName);
//
// void *getProperty(oop object, char *propertyName);
//
// void setProperty(oop object, char *propertyName, void *value);
//
// void *(*getMethod(oop object, char *methodName)) (oop, ...);

#endif

