#ifndef OBJECTS_H
#define OBJECTS_H

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

union object;
typedef union object *oop;

typedef enum Type {
     Undefined ,  Int64 ,  Float64 ,  String ,  Primitive ,  Symbol ,  Object
} Type;

const char *const typeNames[] = {
    "Undefined", "Int64", "Float64", "String", "Primitive", "Symbol", "Object"
};

typedef oop (*prim_t)(oop func, oop self, oop args, oop env);

struct Property { oop key, val; };

#define doProtos(_) _(Object) _(Grammar) _(Definition) _(Assignment) _(Sequence) _(Alternation) _(Star) _(Plus) _(Optional) _(And) _(Not) _(Dot) _(Begin) _(End) _(String) _(CharacterClass) _(Action) _(Identifier)

#define declarePrototype(NAME)    t##NAME,
enum Prototype {
    doProtos(declarePrototype)
};
#undef declarePrototype

struct Int64 {
    Type type;
    long _value;
};

struct Float64 {
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
    struct Int64     Int64;
    struct Float64   Float64;
    struct String    String;
    struct Primitive Primitive;
    struct Symbol    Symbol;
    struct Object    Object;
};

const union object _nil = { Undefined };

#define nil (&_nil)


const char *getTypeName(oop obj);

Type getType(oop obj);

int is(Type type, oop obj);

oop _checkType(oop obj, Type type, char *file, int line);

#define get(OBJ, TYPE, FIELD)         (_checkType(OBJ, TYPE, __FILE__, __LINE__)->TYPE.FIELD)
#define set(OBJ, TYPE, FIELD, VAL)    (_checkType(OBJ, TYPE, __FILE__, __LINE__)->TYPE.FIELD = VAL)

oop make_(size_t size, Type type);

#define make(TYPE)    make_(sizeof(struct TYPE), TYPE)

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

