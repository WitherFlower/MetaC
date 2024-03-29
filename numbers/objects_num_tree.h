union object;

typedef union object *oop;

typedef struct List List;

enum Types {
    Root,
    Binary,
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

struct IntLiteral {
    int type;
    int value;
};

union object {
    int type;
    struct Root Root;
    struct Expression Expression;
    struct Binary Binary;
    struct Ident Ident;
    struct IntLiteral IntLiteral;
};

oop newRoot();

void addExpressionToRoot(oop root, oop expression);

oop newBinary(enum BinaryTypes binaryType, oop leftExpr, oop rightExpr);

oop newIdent(char value);
oop newInteger(int value);

char* getTypeString(enum Types type);

void printTree(oop root);
void evaluateTree(oop root);