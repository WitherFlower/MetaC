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

union Object;

typedef union Object *oop;

// List

typedef struct 
{
    size_t size;
    size_t used; 
    oop data[];
} List;


List *addsize(List *list, size_t inc)
{
    size_t newsize = 0;
    size_t newused = 0;
    
    if(list) {
        newsize = list -> size + inc;
        newused = list -> used;
    }

    list = realloc(list, sizeof(*list) + newsize * sizeof(oop*));
    printf("Reallocating list\n");

    if(list) {
        list -> size = newsize;
        list -> used = newused;
    }

    return list;
}

List *push(List *list, oop object) {
    
    if (list->size == list->used) {
        List *new_list = addsize(list, 32);
        new_list->data[new_list->used] = object;
        new_list->used += 1;
        return new_list;

    } else {
        list->data[list->used] = object;
        list->used += 1;
        return list;

    }
}

List *newList(int size) {
    
    List* list = malloc(sizeof(List) + size * sizeof(oop*));
    
    list->size = size;
    list->used = 0;
    
    return list;
}

enum Types {
    Grammar,
    Declaration,
    Assignment,
    Binary,
    Unary,
    Leaf,
};

char *getTypeString(enum Types type) {
    switch (type) {
        case Grammar: return "Grammar";
        case Declaration: return "Declaration";
        case Assignment: return "Assignment";
        case Binary: return "Binary";
        case Unary: return "Unary";
        case Leaf: return "Leaf";
    }
}

struct Grammar {
    int type;
    List *definitions;
};

struct Declaration {
    int type;
    oop indentifier;
    oop expression;
};

struct Assignment {
    int type;
    oop variable;
    oop rule;
};

struct Binary {
    int type;
    int operator;
    oop leftExpression;
    oop rightExpression;
};

enum BinaryOperators {
    Sequence,
    Alternation,
};

struct Unary {
    int type;
    int operator;
    oop expression;
};

enum UnaryOperators {
    Star,
    Plus,
    Optional,
    And,
    Not,
};

struct Leaf {
    int type;
    int leafType;
    oop expression;
};

enum LeafTypes {
    Dot,
    String,
    CharacterClass,
    Action,
    RuleIdentifier,
};

union Object {
    int type;
    struct Grammar Grammar;
    struct Declaration Declaration;
    struct Assignment Assignment;
    struct Binary Binary;
    struct Unary Unary;
    struct Leaf Leaf;
};

oop _newObject(enum Types type, size_t size) {
    oop this = malloc(size);
    this->type = type;
    return this;
}

oop _checkType(oop object, enum Types type, char *file, int lineNumber) {
    if (object->type != type)
        fatal("[%s:%d] Expected type %s, got %s\n", file, lineNumber, getTypeString(type), getTypeString(object->type));
    return object;
}

#define newObject(TYPE) _newObject(TYPE, sizeof (struct TYPE))
#define get(VAL, TYPE, FIELD) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD
#define set(VAL, TYPE, FIELD, NEW_FIELD_VALUE) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD=NEW_FIELD_VALUE
