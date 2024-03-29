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
    Root,
    Binary,
    Leaf,
    Assign,
    Add,
    Mult,
    Ident,
    IntLiteral,
};

struct Root {
    int type;
    List *expressions;
};

struct Expression {
    int type;    
    oop expression;
};

struct Binary {
    int type;
    oop expression;
};

struct Assign {
    int type;
    oop leftExpr;
    oop rightExpr;
};

struct Add {
    int type;
    oop leftExpr;
    oop rightExpr;
};

struct Mult {
    int type;
    oop leftExpr;
    oop rightExpr;
};

struct Leaf {
    int type;
    oop value;
};

struct IntLiteral {
    int type;
    int value;
};

struct Ident {
    int type;
    char value;
};

union object {
    int type;
    struct Root Root;
    struct Expression Expression;
    struct Binary Binary;
    struct Assign Assign;
    struct Add Add;
    struct Mult Mult;
    struct Leaf Leaf;
    struct IntLiteral IntLiteral;
    struct Ident Ident;
};

oop _newObject(enum Types type, size_t size) {
    oop this = malloc(size);
    this->type = type;
    return this;
}

oop _checkType(oop object, enum Types type, char *file, int lineNumber) {
    if (object->type != type)
        fatal("[%s:%d] Expected type %d, got %d\n", file, lineNumber, type, object->type);
    return object;
}

#define newObject(TYPE) _newObject(TYPE, sizeof (struct TYPE))
#define get(VAL, TYPE, FIELD) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD
#define set(VAL, TYPE, FIELD, NEW_FIELD_VALUE) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD=NEW_FIELD_VALUE

// Tree building functions

void addExpressionToRoot(oop root, oop expression) {
    List *rootExpressions = get(root, Root, expressions);
    set(root, Root, expressions, push(rootExpressions, expression));
}

// Missing intermediary step for type of binary expression
void setBinarySubExpressions(oop binary, oop leftExpr, oop rightExpr) {

    switch (leftExpr->type) {
        case Binary: set(binary, Binary, leftExpr, leftExpr); break;
        case Leaf: set(binary, Leaf, value, leftExpr); break;
        default: fatal("Trying to set object of invalid type in binary expr\n"); break;
    }

    switch (rightExpr->type) {
        case Binary: set(binary, Binary, rightExpr, rightExpr); break;
        case Leaf: set(binary, Leaf, value, rightExpr); break;
        default: fatal("Trying to set object of invalid type in binary expr\n"); break;
    }

}

oop newBinaryExpression(oop leftExpr, enum Types leftType, oop rightExpr, enum Types rightType) {
    oop binary = newObject(Binary);
    setBinarySubExpressions(binary, leftExpr, rightExpr);
    return binary;
}


void setLeafValue(oop leaf, oop value) {
    
}

oop newLeaf(oop )

oop newIntLiteral(int value) {
    oop leaf = newObject(IntLiteral);
    set(leaf, IntLiteral, value, value);
    return leaf;
}

oop newIdent(char value) {
    oop leaf = newObject(Ident);
    set(leaf, Ident, value, value);
    return leaf;
}


// Testing stuff

void testList() {
    printf("Creating list\n");
    List *list = newList(10); 

    for (int i = 0; i < 20; i++) {
        oop newInteger = newObject(IntLiteral);
        set(newInteger, IntLiteral, value, i);

        list = push(list, newInteger);
    }

    for (int i = 0; i < list->used; i++) {
        printf("%d ", get(list->data[i], IntLiteral, value));
    }

    printf("\n");
    
    printf("List size : %ld/%ld\n", list->used, list->size);
}

// End of danger zone

int main() {
    testList();
    return 0;
}