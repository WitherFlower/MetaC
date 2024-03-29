union object;

typedef union object *oop;

typedef struct List List;

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

oop newRoot();

void addExpressionToRoot(oop root, oop expression);

/*
 * Creates an expression with its type and fields
 * Input : (enum Types type, field1, [field2, ...])
*/
oop newExpression(int argc, ...);

oop newIdent(char value);
oop newIntLiteral(int value);

char* getTypeString(enum Types type);

void printTree(oop root);
void evaluateTree(oop root);