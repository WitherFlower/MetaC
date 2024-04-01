
union Object;

typedef union Object *oop;


enum BinaryOperators {
    Sequence,
    Alternation,
    Definition,
    Assignment,
};

enum UnaryOperators {
    Star,
    Plus,
    Optional,
    And,
    Not,
};

oop newGrammar();

void addRuleDefinitionToGrammar(oop grammar, oop definition);

oop newBinary(enum BinaryOperators op, oop leftExpression, oop rightExpression);

oop newUnary(enum UnaryOperators op, oop expression);

oop newDot();

oop newString(char *value);

oop newCharacterClass(char *value);

oop newAction(char *value);

oop newIdentifier(char *value);

void printTree(oop grammar);
