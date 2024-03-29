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
    Assign,
    Add,
    Sub,
    Mult,
    Div,
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

struct Sub {
    int type;
    oop leftExpr;
    oop rightExpr;
};

struct Mult {
    int type;
    oop leftExpr;
    oop rightExpr;
};

struct Div {
    int type;
    oop leftExpr;
    oop rightExpr;
};

struct Ident {
    int type;
    char value;
};

struct IntLiteral {
    int type;
    int value;
};

union object {
    int type;
    struct Root Root;
    struct Expression Expression;
    struct Assign Assign;
    struct Add Add;
    struct Sub Sub;
    struct Mult Mult;
    struct Div Div;
    struct Ident Ident;
    struct IntLiteral IntLiteral;
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

/*
 * Creates an expression with its type and fields
 * Input : (enum Types type, field1, [field2, ...])
*/
oop newExpression(enum Types type, ...) {
    
    va_list args;
    va_start(args, type);
    
    switch (type) {
        case Ident: {
            oop expression = newObject(Ident);

            char value = va_arg(args, int);
            set(expression, Ident, value, value);

            va_end(args);
            return expression;
        }
        case IntLiteral: {
            oop expression = newObject(IntLiteral);

            int value = va_arg(args, int);
            set(expression, IntLiteral, value, value);

            va_end(args);
            return expression;
        }
        case Assign: {
            oop expression = newObject(Assign);

            oop leftExpr = va_arg(args, oop);
            oop rightExpr = va_arg(args, oop);

            set(expression, Assign, leftExpr, leftExpr);
            set(expression, Assign, rightExpr, rightExpr);

            va_end(args);
            return expression;
        }
        case Add: {
            oop expression = newObject(Add);

            oop leftExpr = va_arg(args, oop);
            oop rightExpr = va_arg(args, oop);

            set(expression, Add, leftExpr, leftExpr);
            set(expression, Add, rightExpr, rightExpr);

            va_end(args);
            return expression;
        }
        case Sub: {
            oop expression = newObject(Sub);

            oop leftExpr = va_arg(args, oop);
            oop rightExpr = va_arg(args, oop);

            set(expression, Sub, leftExpr, leftExpr);
            set(expression, Sub, rightExpr, rightExpr);

            va_end(args);
            return expression;
        }
        case Mult: {
            oop expression = newObject(Mult);

            oop leftExpr = va_arg(args, oop);
            oop rightExpr = va_arg(args, oop);

            set(expression, Mult, leftExpr, leftExpr);
            set(expression, Mult, rightExpr, rightExpr);

            va_end(args);
            return expression;
        }
        case Div: {
            oop expression = newObject(Div);

            oop leftExpr = va_arg(args, oop);
            oop rightExpr = va_arg(args, oop);

            set(expression, Div, leftExpr, leftExpr);
            set(expression, Div, rightExpr, rightExpr);

            va_end(args);
            return expression;
        }
        default: {
            va_end(args);
            fatal("Invalid number of members for expression type %d", type);
        }
    }
}

oop newIdent(char value) {
    oop ident = newObject(Ident);
    set(ident, Ident, value, value);
    return ident;
}

oop newIntLiteral(int value) {
    oop intLiteral = newObject(IntLiteral);
    set(intLiteral, IntLiteral, value, value);
    return intLiteral;
}

char* getTypeString(enum Types type) {
    switch (type) {
        case Root: return "Root";
        case Assign: return "Assign";
        case Add: return "Add";
        case Sub: return "Sub";
        case Mult: return "Mult";
        case Div: return "Div";
        case Ident: return "Ident";
        case IntLiteral: return "IntLiteral";
    }
}

void printExpression(oop expression, int depth) {

    printf("%*s| %s\n", 2 * depth, "", getTypeString(expression->type));
    
    switch (expression->type) {
        case Assign:
            printExpression(get(expression, Assign, leftExpr), depth + 1);
            printExpression(get(expression, Assign, rightExpr), depth + 1);
            break;

        case Add:
            printExpression(get(expression, Add, leftExpr), depth + 1);
            printExpression(get(expression, Add, rightExpr), depth + 1);
            break;

        case Sub:
            printExpression(get(expression, Sub, leftExpr), depth + 1);
            printExpression(get(expression, Sub, rightExpr), depth + 1);
            break;

        case Mult:
            printExpression(get(expression, Mult, leftExpr), depth + 1);
            printExpression(get(expression, Mult, rightExpr), depth + 1);
            break;

        case Div:
            printExpression(get(expression, Div, leftExpr), depth + 1);
            printExpression(get(expression, Div, rightExpr), depth + 1);
            break;

        case Ident:
            printf("%*s| %c\n", 2 * (depth + 1), "", get(expression, Ident, value));
            break;

        case IntLiteral:
            printf("%*s| %d\n", 2 * (depth + 1), "", get(expression, IntLiteral, value));
            break;
    }
}

void printTree(oop root) {
    List *expressions = get(root, Root, expressions);
    for (int i = 0; i < expressions->used; i++) {
        printExpression(expressions->data[i], 0);
    }
}

int evaluateExpression(oop expression, int *variables) {
    
    switch (expression->type) {
        case Assign: {
            oop identExpr = get(expression, Assign, leftExpr);
            char ident = get(identExpr, Ident, value);
            int rightExprValue = evaluateExpression(get(expression, Assign, rightExpr), variables);
            variables[ident - 'a'] = rightExprValue;
            return rightExprValue;
        }

        case Add:
            return
                ( evaluateExpression(get(expression, Add, leftExpr), variables)
                + evaluateExpression(get(expression, Add, rightExpr), variables)
                );

        case Sub:
            return
                ( evaluateExpression(get(expression, Sub, leftExpr), variables)
                - evaluateExpression(get(expression, Sub, rightExpr), variables)
                );

        case Mult:
            return
                ( evaluateExpression(get(expression, Mult, leftExpr), variables)
                * evaluateExpression(get(expression, Mult, rightExpr), variables)
                );

        case Div:
            return
                ( evaluateExpression(get(expression, Div, leftExpr), variables)
                / evaluateExpression(get(expression, Div, rightExpr), variables)
                );

        case Ident:
            return variables[get(expression, Ident, value) - 'a'];

        case IntLiteral:
            return get(expression, IntLiteral, value);

        default: fatal("Tried to evaluate invalid type %s\n", getTypeString(expression->type));
    }
}

void evaluateTree(oop root) {
    List *expressions = get(root, Root, expressions);
    int variables[26];

    for (int i = 0; i < expressions->used; i++) {
        int expressionValue = evaluateExpression(expressions->data[i], variables);
        printf("Expression #%d evaluated to %d\n", i, expressionValue);
    }
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

// int main() {
//     testList();
//     return 0;
// }