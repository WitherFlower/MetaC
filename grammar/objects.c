#include "objects.h"

#include <stdio.h>
#include <string.h>

#include "dict.h"

static Dict *Types = NULL;

void initTypes() {
    if (Types == NULL) {
        Types = newDict();
    } else {
        fprintf(stderr, "Error: Tried to init global type list twice\n");
    }
}

int declareType(char *name, Dict *methods) {

    if (get(Types, name) != NULL) {
        fprintf(stderr, "Error: tried to redeclare existing type %s\n", name);
        return 0;
    }

    set(Types, name, methods);
    return 1;
}

oop newObject(char *typeName) {
    oop obj = malloc(sizeof(struct Object));
    obj->properties = newDict();
    obj->methods = get(Types, typeName);
    return obj;
}

void *getProperty(oop object, char *propertyName) {
    return get(object->properties, propertyName);
}

void *getMethod(oop object, char *methodName) {
    return get(object->methods, methodName);
}

