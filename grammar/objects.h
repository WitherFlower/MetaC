#ifndef OBJECTS_H
#define OBJECTS_H
#include "dict.h"

static Dict *Types;

void initTypes();

int declareType(char *name, Dict *methods);

struct Object {
    char *type;
    Dict *properties;
    Dict *methods;
};
typedef struct Object *oop;

oop newObject(char *typeName);

void *getProperty(oop object, char *propertyName);

void setProperty(oop object, char *propertyName, void *value);

void *(*getMethod(oop object, char *methodName)) (oop, ...);

#endif

