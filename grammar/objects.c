#include "objects.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

void fatal(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "\n");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

const char *getTypeName(oop obj) {
    int type = getType(obj);            assert(0 <= type && type <= indexableSize(typeNames));
    return typeNames[type];
}

oop _checkType(oop obj, enum Type type, char *file, int line) {
    if (getType(obj) != type) fatal("%s:%d: expected type %d, got %d\n", file, line, type, getType(obj));
    return obj;
}

enum Type getType(oop obj) {
# if TAGS
    if ((intptr_t)obj & TAGMASK) return ((intptr_t)obj & TAGMASK);
# endif
    return obj->type;
}

int is(enum Type type, oop obj)    { return type == getType(obj); }

oop make_(size_t size, enum Type type) {
    oop obj = xmalloc(size);
    obj->type = type;
    return obj;
}

void *getProperty(oop object, char *propertyName) {
    return get(object->properties, propertyName);
}

void setProperty(oop object, char *propertyName, void *value) {
    set(object->properties, propertyName, value);
}

// Return type of returned function (*name of HOF (args of HOF)) (args of returned function)
void *(*getMethod(oop object, char *methodName)) (oop, ...) {
    return get(object->methods, methodName);
}

