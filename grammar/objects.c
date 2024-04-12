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
    Dict *methods = get(Types, typeName);

    if (methods == NULL) {
        fprintf(stderr, "Tried creating an object of inexistant type %s.\n", typeName);
        exit(1);
    }

    oop obj = malloc(sizeof(struct Object));
    obj->type = typeName;
    obj->properties = newDict();
    obj->methods = methods;
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

