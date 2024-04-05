#ifndef GRAMMAR_OBJECTS_H
#define GRAMMAR_OBJECTS_H

union Object;

typedef union Object *oop;

enum BinaryOperators {
    Sequence,
    Alternation,
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

oop newDefinition(oop name, oop rule);

oop newAssignment(oop variableName, oop ruleIdentifier);

oop newBinary(enum BinaryOperators op, oop leftExpression, oop rightExpression);

oop newUnary(enum UnaryOperators op, oop expression);

oop newDot();

oop newBegin();

oop newEnd();

oop newString(char *value);

oop newCharacterClass(char *value);

oop newAction(char *value);

oop newIdentifier(char *value);

oop newInteger(int value);

oop newSymbol(char *string);

int objectEquals(oop obj, oop other);

void printTree(oop grammar);

void writeTree(oop grammar);

#endif

