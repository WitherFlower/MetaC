#ifndef OBJECTS_H
#define OBJECTS_H
#include "dict.h"

static Dict *Types;

void initTypes();

int declareType(char *name, Dict *methods);

struct Object {
    Dict *properties;
    Dict *methods;
};
typedef struct Object *oop;

oop newObject(char *typeName);

void *getProperty(oop object, char *propertyName);

void *getMethod(oop object, char *methodName);

#endif

