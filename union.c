#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void fatal(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "\n");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

union object;

typedef union object *oop;

enum Types {
    Int,
    Float,
};

struct Int {
    int type;
    int value;
};

struct Float {
    int type;
    float value;
};

union object {
    int type;
    struct Int Int;
    struct Float Float;
};

oop _newObject(enum Types type, size_t size) {
    oop this = malloc(size);
    this->type = type;
    return this;
}

oop _checkType(oop value, enum Types type, char *file, int lineNumber) {
    if (value->type != type)
        fatal("[%s:%d] Expected type %d, got %d\n", file, lineNumber, type, value->type);
    return value;
}

#define newObject(TYPE) _newObject(TYPE, sizeof (struct TYPE))
#define get(VAL, TYPE, FIELD) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD
#define set(VAL, TYPE, FIELD, NEW_FIELD_VALUE) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD=NEW_FIELD_VALUE

oop newInt(int value) {
    oop this = newObject(Int);
    this->Int.value = value;
    return this;
}

oop newFloat(float value) {
    oop this = newObject(Float);
    this->Float.value = value;
    return this;
}

void print(oop o) {
    switch (o->type) {
        case Int: printf("%08x\n", get(o, Int, value)); break;
        case Float: printf("%f\n", get(o, Float, value)); break;
    }
}

int main() {
    oop o;

    o = newInt(42);
    print(o);

    // Memory problem ?
    o = newFloat(3.1415);
    print(o);

    set(o, Float, value, 1.414);
    print(o);
    
    // Use for the tree printing
    for (int i = 0; i < 10; i++) {
        printf("%*s|\n", 2*i, "");
    }
}
