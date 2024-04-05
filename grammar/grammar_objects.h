#ifndef GRAMMAR_OBJECTS_H
#define GRAMMAR_OBJECTS_H

#include <stdlib.h>

union Object;

typedef union Object *oop;

typedef struct 
{
    size_t size;
    size_t used; 
    oop data[];
} List;

enum Types {
    Grammar,
    Definition,
    Assignment,
    Binary,
    Unary,
    Dot,
    Begin,
    End,
    String,
    CharacterClass,
    Action,
    Identifier,
    Symbol,
};

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

struct Grammar {
    int type;
    List *definitions;
};


struct Definition {
    int type;
    oop name;
    oop rule;
};

struct Assignment {
    int type;
    oop variableName;
    oop ruleIdentifier;
};

struct Binary {
    int type;
    int op;
    oop leftExpression;
    oop rightExpression;
};

struct Unary {
    int type;
    int op;
    oop expression;
};

struct Dot {
    int type;
};

struct Begin {
    int type;
};

struct End {
    int type;
};

struct String {
    int type;
    char *value;
};

struct CharacterClass {
    int type;
    char *value;
};

struct Action {
    int type;
    char *value;
};

struct Identifier {
    int type;
    char *value;
};

struct Symbol {
    int type;
    char *string;
};


union Object {
    int type;
    struct Grammar Grammar;
    struct Definition Definition;
    struct Assignment Assignment;
    struct Binary Binary;
    struct Unary Unary;
    struct Dot Dot;
    struct Begin Begin;
    struct End End;
    struct String String;
    struct CharacterClass CharacterClass;
    struct Action Action;
    struct Identifier Identifier;
    struct Symbol Symbol;
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

