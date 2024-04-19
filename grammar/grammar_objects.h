#ifndef GRAMMAR_OBJECTS_H
#define GRAMMAR_OBJECTS_H

#include <stdlib.h>
#include "list.h"
#include "objects.h"

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

void declareTypes();

oop newGrammar();

oop newDefinition(oop name, oop rule);

oop newAssignment(oop variableName, oop ruleIdentifier);

oop newSequence(oop leftExpression, oop rightExpression);

oop newAlternation(oop leftExpression, oop rightExpression);

oop newStar(oop expression);

oop newPlus(oop expression);

oop newOptional(oop expression);

oop newAnd(oop expression);

oop newNot(oop expression);

oop newDot();

oop newBegin();

oop newEnd();

oop newString(char *value);

oop newCharacterClass(char *value);

oop newAction(char *value, void (*function)());

oop newIdentifier(char *value);

oop newSymbol(char *string);

oop addSymbolToSymbolList(List *symbolList, oop symbol);

oop addNewStringToSymbolList(List *symbolList, char *string);

int equals(oop obj, oop other);

void printTree(oop grammar);

void writeTree(oop grammar);

oop evaluateTree(oop grammar, char *string);

#endif

