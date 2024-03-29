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
    Ident,
    Integer,
};

struct Root {
    int type;
    List *expressions;
};

struct Binary {
    int type;
    int binaryType;
    oop leftExpr;
    oop rightExpr;
};

enum BinaryTypes {
    Assign,
    Add,
    Sub,
    Mult,
    Div,
};

struct Ident {
    int type;
    char value;
};

struct Integer {
    int type;
    int value;
};

union object {
    int type;
    struct Root Root;
    struct Binary Binary;
    struct Ident Ident;
    struct Integer Integer;
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

oop newRoot() {
    oop root = newObject(Root);
    set(root, Root, expressions, newList(10));
    return root;
}

oop newBinary(enum BinaryTypes binaryType, oop leftExpr, oop rightExpr) {
    oop binary = newObject(Binary);
    set(binary, Binary, binaryType, binaryType);
    set(binary, Binary, leftExpr, leftExpr);
    set(binary, Binary, rightExpr, rightExpr);
    return binary;
}

oop newIdent(char value) {
    oop ident = newObject(Ident);
    set(ident, Ident, value, value);
    return ident;
}

oop newInteger(int value) {
    oop intLiteral = newObject(Integer);
    set(intLiteral, Integer, value, value);
    return intLiteral;
}

char* getTypeString(enum Types type) {
    switch (type) {
        case Root: return "Root";
        case Binary: return "Binary";
        case Ident: return "Ident";
        case Integer: return "Integer";
    }
}

char* getBinaryTypeString(enum BinaryTypes type) {
    switch (type) {
        case Assign: return "Assign";
        case Add: return "Add";
        case Sub: return "Sub";
        case Mult: return "Mult";
        case Div: return "Div";
    }
}

void printExpression(oop expression, int depth) {

    switch (expression->type) {
        case Binary:
            printf(
                "%*s| %s (%s)\n",
                2 * depth,
                "",
                getTypeString(expression->type),
                getBinaryTypeString(get(expression, Binary, binaryType))
            );

            printExpression(get(expression, Binary, leftExpr), depth + 1);
            printExpression(get(expression, Binary, rightExpr), depth + 1);

            break;

        case Ident:
            printf("%*s| %s (%c)\n", 2 * depth, "", getTypeString(Ident), get(expression, Ident, value));
            break;

        case Integer:
            printf("%*s| %s (%d)\n", 2 * depth, "", getTypeString(Integer), get(expression, Integer, value));
            break;
    }

}

void printTree(oop root) {
    List *expressions = get(root, Root, expressions);
    for (int i = 0; i < expressions->used; i++) {
        printExpression(expressions->data[i], 0);
    }
}

oop evaluateExpression(oop expression, oop *variables) {
    
    switch (expression->type) {
        case Binary: {
                         
            oop leftValue = evaluateExpression(get(expression, Binary, leftExpr), variables);
            oop rightValue = evaluateExpression(get(expression, Binary, rightExpr), variables);
            oop newValue;

            switch (get(expression, Binary, binaryType)) {

                case Assign: {
                    oop identExpr = get(expression, Binary, leftExpr);
                    char ident = get(identExpr, Ident, value);
                    variables[ident - 'a'] = rightValue;
                    return rightValue;
                }

                case Add:
                    newValue = newObject(Integer);
                    set(newValue, Integer, value, get(leftValue, Integer, value) + get(rightValue, Integer, value));
                    return newValue;
                case Sub:
                    newValue = newObject(Integer);
                    set(newValue, Integer, value, get(leftValue, Integer, value) - get(rightValue, Integer, value));
                    return newValue;
                case Mult:
                    newValue = newObject(Integer);
                    set(newValue, Integer, value, get(leftValue, Integer, value) * get(rightValue, Integer, value));
                    return newValue;
                case Div:
                    newValue = newObject(Integer);
                    set(newValue, Integer, value, get(leftValue, Integer, value) / get(rightValue, Integer, value));
                    return newValue;

            }
        }

        case Ident:
            return variables[get(expression, Ident, value) - 'a'];

        case Integer:
            return expression;

        default: fatal("Tried to evaluate invalid type %s\n", getTypeString(expression->type));
    }
}

void evaluateTree(oop root) {
    List *expressions = get(root, Root, expressions);
    oop variables[26];

    for (int i = 0; i < expressions->used; i++) {
        int expressionValue = get(evaluateExpression(expressions->data[i], variables), Integer, value);
        printf("Expression #%d evaluated to %d\n", i, expressionValue);
    }
}

// Testing stuff

void testList() {
    printf("Creating list\n");
    List *list = newList(10); 

    for (int i = 0; i < 20; i++) {
        oop newInteger = newObject(Integer);
        set(newInteger, Integer, value, i);

        list = push(list, newInteger);
    }

    for (int i = 0; i < list->used; i++) {
        printf("%d ", get(list->data[i], Integer, value));
    }

    printf("\n");
    
    printf("List size : %ld/%ld\n", list->used, list->size);
}

// End of danger zone

// int main() {
//     testList();
//     return 0;
// }