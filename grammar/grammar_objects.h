#ifndef GRAMMAR_OBJECTS_H
#define GRAMMAR_OBJECTS_H

#include <stdlib.h>
#include "list.h"

union Object;

typedef union Object *oop;

typedef struct {
    List *varNames;
    List *vars;
    oop returnValue;
    char *input;
} Context;

typedef struct {
    char *string;
    int cursor;
    int lastBegin;
    List *callStack;
} ReadState;

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
    void (*function)(Context *);
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

oop _newObject(enum Types type, size_t size);

oop _checkType(oop object, enum Types type, char *file, int lineNumber);

#define newObject(TYPE) _newObject(TYPE, sizeof (struct TYPE))
#define get(VAL, TYPE, FIELD) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD
#define set(VAL, TYPE, FIELD, NEW_FIELD_VALUE) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD=NEW_FIELD_VALUE

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

oop newAction(char *value, void (*function)());

oop newIdentifier(char *value);

oop newInteger(int value);

oop newSymbol(char *string);

oop addSymbolToSymbolList(List *symbolList, oop symbol);

oop addNewStringToSymbolList(List *symbolList, char *string);

int objectEquals(oop obj, oop other);

void printTree(oop grammar);

void writeTree(oop grammar);

oop evaluateTree(oop grammar, char *string);

#endif

