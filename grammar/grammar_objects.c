#include "grammar_objects.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <strings.h>
#include <regex.h>

#include "dict.h"
#include "list.h"
#include "objects.h"
#include "wonkyregex.h"

/// Utils

void fatal(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "\n");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

char *convertSpecialChars(char *sourceString) {
    char *convertedString = malloc(sizeof(char) * 2 * strlen(sourceString));
    convertedString[0] = 0;

    for (int i = 0; i < strlen(sourceString); i++) {
        
        char current = sourceString[i];

        switch (current) {

            case '\'': { char currentString[] = { '\\', '\'', '\0' }; strcat(convertedString, currentString); break; }
            case '\"': { char currentString[] = { '\\', '\"', '\0' }; strcat(convertedString, currentString); break; }
            case '\\': { char currentString[] = { '\\', '\\', '\0' }; strcat(convertedString, currentString); break; }
            case '\r': { char currentString[] = { '\\', 'r', '\0' }; strcat(convertedString, currentString); break; }
            case '\t': { char currentString[] = { '\\', 't', '\0' }; strcat(convertedString, currentString); break; }
            case '\n': { char currentString[] = { '\\', 'n', '\0' }; strcat(convertedString, currentString); break; }
            default:   { char currentString[] = { current, '\0' }; strcat(convertedString, currentString); break; }
        
        }

    }
    return convertedString;
}

unsigned int getConvertedStringRealLength(char *string) {
    int count = 0;
    for (int i = 0; i < strlen(string); i++){
        if(string[i] == '\\') {
            i++;
        }
        count++;
    }
    return count;
}

typedef struct FileWriter {
    char *filename;
    char *actionSection;
    char *declarationSection;
} FileWriter;

FileWriter *newFileWriter(char *filename) {
    FileWriter *fw = malloc(sizeof(FileWriter));
    fw->filename = filename;
    fw->actionSection = calloc(sizeof(char), 1);
    fw->declarationSection = calloc(sizeof(char), 1);
    return fw;
}

void appendToActionSection(FileWriter *fileWriter, char *content) {
    fileWriter->actionSection = realloc(
            fileWriter->actionSection,
            sizeof(char) * (strlen(fileWriter->actionSection) + strlen(content) + 1)
            );
    strcat(fileWriter->actionSection, content);
}

void appendToDeclarationSection(FileWriter *fileWriter, char *content) {
    fileWriter->declarationSection = realloc(
            fileWriter->declarationSection,
            sizeof(char) * (strlen(fileWriter->declarationSection) + strlen(content) + 1)
            );
    strcat(fileWriter->declarationSection, content);
}

void writeToFile(FileWriter *fileWriter) {
    FILE *fptr = fopen("grammar_get_tree.c", "w");
    fprintf(fptr, "/// Actions\n\n%s\n", fileWriter->actionSection);
    fprintf(fptr, "/// Declarations\n\n%s\n", fileWriter->declarationSection);
    fclose(fptr);
}

void freeFileWriter(FileWriter *fileWriter) {
    free(fileWriter->actionSection);
    free(fileWriter->declarationSection);
    free(fileWriter);
}

void noOpMethod (oop object, ...) { }

Context *newContext(char *currentInput) {
    Context *ctx = malloc(sizeof(Context));
    ctx->varNames = newList(10);
    ctx->vars = newList(10);
    ctx->input = currentInput;
    ctx->returnValue = NULL;
    return ctx;
}

/// ---- Class declarations ----

/// Grammar Class

oop newGrammar() {
    oop grammar = newObject("Grammar");
    setProperty(grammar, "definitions", newList(10));
    return grammar;
}

void addRuleDefinition(oop grammar, oop definition) {
    // printf("Adding Rule to grammar\n");
    List *rootExpressions = getProperty(grammar, "definitions");
    push(rootExpressions, definition);
}

void printGrammar(oop grammar, int depth) {
    List *definitions = getProperty(grammar, "definitions");
    for (int i = 0; i < definitions->used; i++) {
        getMethod(definitions->data[i], "print")(definitions->data[i], depth);
    }
}

Dict *getGrammarMethods() {
    Dict *methods = newDict();
    set(methods, "addRuleDefinition", addRuleDefinition);
    set(methods, "print", printGrammar);
    return methods;
}

/// Definition Class

oop newDefinition(oop name, oop rule) {
    oop definition = newObject("Definition");
    setProperty(definition, "name", name);
    setProperty(definition, "rule", rule);

    return definition;
}

void printDefinition(oop definition, int depth) {
    printf("%*s| %s (%s)\n", 2 * depth, "", definition->type,
            (char*)getProperty(getProperty(definition, "name"), "value"));
    oop innerRule = getProperty(definition, "rule");
    getMethod(innerRule, "print")(innerRule, depth + 1);
}

char *writeDefinition(oop definition, FileWriter *fileWriter, int *declarationCount, List *localVars) {

    oop name = getProperty(definition, "name");
    char *nameVarName = getMethod(name, "write")(name, fileWriter, declarationCount, localVars);

    List *l = newList(10);

    oop rule = getProperty(definition, "rule");
    char *ruleVarName = getMethod(rule, "write")(rule, fileWriter, declarationCount, l);

    for (int i = 0; i < l->used; i++) {
        free(l->data[i]);
    }

    free(l);

    const char *type = definition->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName) + strlen(nameVarName) + strlen(ruleVarName)));
    sprintf(outputString, "oop %s = newDefinition(%s, %s);\n",
            fullVariableName, nameVarName, ruleVarName);
    appendToDeclarationSection(fileWriter, outputString);

    return fullVariableName;
}

Dict *getDefinitionMethods() {
    Dict *methods = newDict();
    set(methods, "print", printDefinition);
    set(methods, "write", writeDefinition);
    return methods;
}

/// Assignment Class

oop newAssignment(oop variableName, oop ruleIdentifier) {

    oop assignment = newObject("Assignment");
    setProperty(assignment, "variableName", variableName);
    setProperty(assignment, "ruleIdentifier", ruleIdentifier);

    return assignment;
}

void printAssignment(oop assignment, int depth) {
    printf("%*s| %s (%s)\n", 2 * depth, "", assignment->type,
            (char*)getProperty(getProperty(assignment, "variableName"), "value"));
    oop innerExpression = getProperty(assignment, "ruleIdentifier");
    getMethod(innerExpression, "print")(innerExpression, depth + 1);
}

char *writeAssignment(oop assignment, FileWriter *fileWriter, int *declarationCount, List *localVars) {

    oop assignedVariable = getProperty(assignment, "variableName");
    char *assignedVariableName = getMethod(assignedVariable, "write")(
            assignedVariable, fileWriter, declarationCount, localVars);

    char *assignedVariableStringName = getProperty(assignedVariable, "value");
    addNewStringToSymbolList(localVars, assignedVariableStringName);

    oop ruleIdentifier = getProperty(assignment, "ruleIdentifier");
    char *ruleIdentifierName = getMethod(ruleIdentifier, "write")(
            ruleIdentifier, fileWriter, declarationCount, localVars);

    const char *type = assignment->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName) + strlen(assignedVariableName) + strlen(ruleIdentifierName)));
    sprintf(outputString, "oop %s = newAssignment(%s, %s);\n",
            fullVariableName, assignedVariableName, ruleIdentifierName);
    appendToDeclarationSection(fileWriter, outputString);

    return fullVariableName;
}

void fillContextAssignment(oop rule, Context *ctx) {
    // printf("Adding variable %s to context\n", get(get(rule, Assignment, variableName), String, value));
    addNewStringToSymbolList(ctx->varNames, getProperty(getProperty(rule, "variableName"), "value"));
}

long applyAssignment(oop rule, List *ruleNames, List *rules, ReadState *state, Context *ctx) {

    oop s = newSymbol(getProperty(getProperty(rule, "ruleIdentifier"), "value"));
    int ruleIndex = indexOfByValue(ruleNames, s);

    oop innerRule = rules->data[ruleIndex];
    Context *innerContext = newContext(ctx->input);

    // printf("Applying the following inner rule\n");
    // getMethod(innerRule, "print")(innerRule, 0);

    push(state->callStack, s);

    int cursor = state->cursor;
    int lastBegin = state->lastBegin;

    getMethod(innerRule, "fillContext")(innerRule, innerContext);

    long success = (long)getMethod(innerRule, "applyRule")(innerRule, ruleNames, rules, state, innerContext);

    if (success) {
        oop varSymbol = newSymbol(getProperty(getProperty(rule, "variableName"), "value"));

        // printf("\nSuccessfully parsed assignment to variable %s\n", (char*)getProperty(varSymbol, "string"));
        // printf("Value :\n");
        // getMethod(innerContext->returnValue, "print")(innerContext->returnValue, 0);
        // printf("Storing Value in vars[%d]\n", indexOfByValue(ctx->varNames, varSymbol));

        ctx->vars->data[indexOfByValue(ctx->varNames, varSymbol)] = innerContext->returnValue;

        free(varSymbol);

    } else {
        state->cursor = cursor;
        state->lastBegin = lastBegin;
    }

    state->callStack->used--;
    free(s);
    return success;
}

Dict *getAssignmentMethods() {
    Dict *methods = newDict();
    set(methods, "print", printAssignment);
    set(methods, "write", writeAssignment);
    set(methods, "fillContext", fillContextAssignment);
    set(methods, "applyRule", applyAssignment);
    return methods;
}

/// Generic Binary Class Methods

char *writeBinary(oop binary, FileWriter *fileWriter, int *declarationCount, List *localVars) {
    oop leftExpression = getProperty(binary, "leftExpression");
    char *leftExpressionVarName = getMethod(leftExpression, "write")(
            leftExpression, fileWriter, declarationCount, localVars);

    oop rightExpression = getProperty(binary, "rightExpression");
    char *rightExpressionVarName = getMethod(rightExpression, "write")(
            rightExpression, fileWriter, declarationCount, localVars);
    
    const char *type = binary->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName) + strlen(leftExpressionVarName) + strlen(rightExpressionVarName)));
    sprintf(outputString, "oop %s = new%s(%s, %s);\n",
            fullVariableName, type, leftExpressionVarName, rightExpressionVarName);
    appendToDeclarationSection(fileWriter, outputString);

    return fullVariableName;
}

void fillContextBinary(oop rule, Context *ctx) {
    oop leftExpression = getProperty(rule, "leftExpression");
    getMethod(leftExpression, "fillContext")(leftExpression, ctx);

    oop rightExpression = getProperty(rule, "rightExpression");
    getMethod(rightExpression, "fillContext")(rightExpression, ctx);
}

/// Sequence Class

oop newSequence(oop leftExpression, oop rightExpression) {
    oop sequence = newObject("Sequence");
    setProperty(sequence, "leftExpression", leftExpression);
    setProperty(sequence, "rightExpression", rightExpression);
    return sequence;
}

void printSequence(oop sequence, int depth) {
    printf(
            "%*s| %s\n",
            2 * depth,
            "",
            sequence->type
          );

    oop leftExpr = getProperty(sequence, "leftExpression");
    getMethod(leftExpr, "print")(leftExpr, depth + 1);

    oop rightExpr = getProperty(sequence, "rightExpression");
    getMethod(rightExpr, "print")(rightExpr, depth + 1);
}

long applySequence(oop sequence, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop leftExpr = getProperty(sequence, "leftExpression");
    oop rightExpr = getProperty(sequence, "rightExpression");

    if (getMethod(leftExpr, "applyRule")(leftExpr, ruleNames, rules, state, ctx)) {
        return (long)getMethod(rightExpr, "applyRule")(rightExpr, ruleNames, rules, state, ctx);
    }
    return 0;
}

Dict *getSequenceMethods() {
    Dict *methods = newDict();
    set(methods, "print", printSequence);
    set(methods, "write", writeBinary);
    set(methods, "fillContext", fillContextBinary);
    set(methods, "applyRule", applySequence);
    return methods;
}

/// Alternation Class

oop newAlternation(oop leftExpression, oop rightExpression) {
    oop alternation = newObject("Alternation");
    setProperty(alternation, "leftExpression", leftExpression);
    setProperty(alternation, "rightExpression", rightExpression);
    return alternation;
}

void printAlternation(oop alternation, int depth) {
    printf(
            "%*s| %s\n",
            2 * depth,
            "",
            alternation->type
          );

    oop leftExpr = getProperty(alternation, "leftExpression");
    getMethod(leftExpr, "print")(leftExpr, depth + 1);

    oop rightExpr = getProperty(alternation, "rightExpression");
    getMethod(rightExpr, "print")(rightExpr, depth + 1);
}

long applyAlternation(oop alternation, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop leftExpr = getProperty(alternation, "leftExpression");
    oop rightExpr = getProperty(alternation, "rightExpression");

    int cursor = state->cursor;
    int lastBegin = state->lastBegin;

    if (!getMethod(leftExpr, "applyRule")(leftExpr, ruleNames, rules, state, ctx)) {
        // printf("Going to next alternation choice\n");
        state->cursor = cursor;
        state->lastBegin = lastBegin;
        return (long)getMethod(rightExpr, "applyRule")(rightExpr, ruleNames, rules, state, ctx);
    }
    return 1;
}

Dict *getAlternationMethods() {
    Dict *methods = newDict();
    set(methods, "print", printAlternation);
    set(methods, "write", writeBinary);
    set(methods, "fillContext", fillContextBinary);
    set(methods, "applyRule", applyAlternation);
    return methods;
}

/// Generic Unary Class Methods

void printUnary(oop unary, int depth) {
    printf(
            "%*s| %s\n",
            2 * depth,
            "",
            unary->type
          );
    oop expression = getProperty(unary, "expression");
    getMethod(expression, "print")(expression, depth + 1);
}

char *writeUnary(oop unary, FileWriter *fileWriter, int *declarationCount, List *localVars) {
    oop expression = getProperty(unary, "expression");
    char *expressionVarName = getMethod(expression, "write")(
            expression, fileWriter, declarationCount, localVars);

    const char *type = unary->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName) + strlen(expressionVarName)));
    sprintf(outputString, "oop %s = new%s(%s);\n",
            fullVariableName, type, expressionVarName);
    appendToDeclarationSection(fileWriter, outputString);

    return fullVariableName;
}

void fillContextUnary(oop rule, Context *ctx) {
    oop expression = getProperty(rule, "expression");
    getMethod(expression, "fillContext")(expression, ctx);
}

/// Star Class

oop newStar(oop expression) {
    oop star = newObject("Star");
    setProperty(star, "expression", expression);
    return star;
}

long applyStar(oop star, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop innerExpression = getProperty(star, "expression");
    while (getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx));
    return 1;
}

Dict *getStarMethods() {
    Dict *methods = newDict();
    set(methods, "print", printUnary);
    set(methods, "write", writeUnary);
    set(methods, "fillContext", fillContextUnary);
    set(methods, "applyRule", applyStar);
    return methods;
}

/// Plus Class

oop newPlus(oop expression) {
    oop plus = newObject("Plus");
    setProperty(plus, "expression", expression);
    return plus;
}

long applyPlus(oop plus, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop innerExpression = getProperty(plus, "expression");

    if (!getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx))
        return 0;

    while (getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx));
    return 1;
}

Dict *getPlusMethods() {
    Dict *methods = newDict();
    set(methods, "print", printUnary);
    set(methods, "write", writeUnary);
    set(methods, "fillContext", fillContextUnary);
    set(methods, "applyRule", applyPlus);
    return methods;
}

/// Optional Class

oop newOptional(oop expression) {
    oop optional = newObject("Optional");
    setProperty(optional, "expression", expression);
    return optional;
}

long applyOptional(oop optional, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop innerExpression = getProperty(optional, "expression");

    getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx);
    return 1;
}

Dict *getOptionalMethods() {
    Dict *methods = newDict();
    set(methods, "print", printUnary);
    set(methods, "write", writeUnary);
    set(methods, "fillContext", fillContextUnary);
    set(methods, "applyRule", applyOptional);
    return methods;
}

/// And Class

oop newAnd(oop expression) {
    oop and = newObject("And");
    setProperty(and, "expression", expression);
    return and;
}

long applyAnd(oop and, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    // Not sure what to do here
    oop innerExpression = getProperty(and, "expression");

    int cursor = state->cursor;
    if (getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx)) {
        state->cursor = cursor;
        return (long)getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx);
    }
    return 0;
}

Dict *getAndMethods() {
    Dict *methods = newDict();
    set(methods, "print", printUnary);
    set(methods, "write", writeUnary);
    set(methods, "fillContext", fillContextUnary);
    set(methods, "applyRule", applyAnd);
    return methods;
}

/// Not Class

oop newNot(oop expression) {
    oop not = newObject("Not");
    setProperty(not, "expression", expression);
    return not;
}

long applyNot(oop not, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop innerExpression = getProperty(not, "expression");

    int cursor = state->cursor;
    // printf("Applying Not with cursor = %d\n", cursor);

    if (getMethod(innerExpression, "applyRule")(innerExpression, ruleNames, rules, state, ctx)) {
        // printf("Exiting Not\n");
        state->cursor = cursor;
        return 0;
    } else {
        return 1;
    }
}

Dict *getNotMethods() {
    Dict *methods = newDict();
    set(methods, "print", printUnary);
    set(methods, "write", writeUnary);
    set(methods, "fillContext", fillContextUnary);
    set(methods, "applyRule", applyNot);
    return methods;
}

/// Generic Empty Leaf Class Methods

void printEmptyLeaf(oop leaf, int depth) {
    printf("%*s| %s\n", 2 * depth, "", leaf->type);
}

char *writeEmptyLeaf(oop leaf, FileWriter *fileWriter, int *declarationCount, List *localVars) {
    char *type = leaf->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName)));
    sprintf(outputString, "oop %s = new%s();", fullVariableName, type);
    appendToDeclarationSection(fileWriter, outputString);

    return fullVariableName;
}

/// Dot Class

oop newDot() {
    return newObject("Dot");
}

long applyDot(oop dot, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    if (state->string[state->cursor] != '\0') {
        state->cursor++;
        return 1;
    }
    return 0;
}

Dict *getDotMethods() {
    Dict *methods = newDict();
    set(methods, "print", printEmptyLeaf);
    set(methods, "write", writeEmptyLeaf);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyDot);
    return methods;
}

/// Begin Class

oop newBegin() {
    return newObject("Begin");
}

long applyBegin(oop begin, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    state->lastBegin = state->cursor;
    return 1;
}

Dict *getBeginMethods() {
    Dict *methods = newDict();
    set(methods, "print", printEmptyLeaf);
    set(methods, "write", writeEmptyLeaf);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyBegin);
    return methods;
}

/// End Class

oop newEnd() {
    return newObject("End");
}

long applyEnd(oop end, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    if (ctx->input != NULL) {
        free(ctx->input);
    }
    unsigned int inputSize = state->cursor - state->lastBegin;
    ctx->input = calloc(sizeof(char), state->cursor - state->lastBegin + 1);
    strncpy(ctx->input, state->string + state->lastBegin, inputSize);
    return 1;
}

Dict *getEndMethods() {
    Dict *methods = newDict();
    set(methods, "print", printEmptyLeaf);
    set(methods, "write", writeEmptyLeaf);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyEnd);
    return methods;
}

/// Generic String Leaf Class Methods

void printStringLeaf(oop expression, int depth) {
    printf("%*s| %s (%s)\n", 2 * depth, "", expression->type, (char*)getProperty(expression, "value"));
}

char *writeStringLeaf(oop expression, FileWriter *fileWriter, int *declarationCount, List *localVars) {
    char *type = expression->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *value = getProperty(expression, "value");

    char *convertedValue = convertSpecialChars(value);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName) + strlen(convertedValue)));
    sprintf(outputString, "oop %s = new%s(\"%s\");\n",
            fullVariableName, type, convertedValue);
    appendToDeclarationSection(fileWriter, outputString);

    free(convertedValue);

    return fullVariableName;
}

/// String Class

oop newString(char *value) {
    // printf("Creating new String with value %s\n", value);
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop string = newObject("String");
    setProperty(string, "value", newValue);
    return string;
}

long applyString(oop string, List *ruleNames, List *rules, ReadState *state, Context *ctx) {

    int n = strlen(getProperty(string, "value"));
    char *received = calloc(1 + n, sizeof(char));
    strncpy(received, state->string + state->cursor, n);
    received = convertSpecialChars(received);
    received[n] = '\0';

    // printf("Received String : \"%s\"\n", received);

    char *expected = getProperty(string, "value");
    // printf("Expected String : \"%s\"\n", get(rule, String, value));

    if (strcmp(received, expected) == 0) {
        state->cursor += getConvertedStringRealLength(getProperty(string, "value"));
        // printf("Equal, incrementing cursor by %u\n", getConvertedStringRealLength(get(rule, String, value)));
        free(received);
        return 1;
    }
    // printf("Not Equal\n");
    free(received);
    return 0;
}

Dict *getStringMethods() {
    Dict *methods = newDict();
    set(methods, "print", printStringLeaf);
    set(methods, "write", writeStringLeaf);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyString);
    return methods;
}

/// CharacterClass Class

oop newCharacterClass(char *value) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop characterClass = newObject("CharacterClass");
    setProperty(characterClass, "value", newValue);
    return characterClass;
}

long applyCharacterClass(oop characterClass, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    char *classRegex = malloc(sizeof(char) * (3 + strlen(getProperty(characterClass, "value"))));
    sprintf(classRegex, "[%s]", (char*)getProperty(characterClass, "value"));

    // printf("classRegex value : %s\n", classRegex);
    
    char nextChar[] = { state->string[state->cursor], '\0' };

    // printf("nextChar: %c\n", nextChar[0]);

    if (match(classRegex, nextChar)) {
        state->cursor++;
        return 1;
    }

    return 0;
}

Dict *getCharacterClassMethods() {
    Dict *methods = newDict();
    set(methods, "print", printStringLeaf);
    set(methods, "write", writeStringLeaf);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyCharacterClass);
    return methods;
}

/// Action Class

oop newAction(char *value, void (*function)(Context*)) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop action = newObject("Action");
    setProperty(action, "value", newValue);

    setProperty(action, "function", function);
    return action;
}

char *writeAction(oop action, FileWriter *fileWriter, int *declarationCount, List *localVars) {
    char *type = action->type;

    (*declarationCount)++;

    char *fullVariableName = malloc(sizeof(char) * 64);
    sprintf(fullVariableName, "%s%d", type, *declarationCount);

    char *functionName = malloc(sizeof(char) * 64);
    sprintf(functionName, "%s%d_function", type, *declarationCount);

    char *rawActionContent = getProperty(action, "value");

    char *functionDeclaration = calloc(sizeof(char), 65536);
    unsigned int cursor = 0;
    cursor += sprintf(functionDeclaration + cursor, "\n");
    cursor += sprintf(functionDeclaration + cursor, "#define $$ ctx->returnValue\n");
    cursor += sprintf(functionDeclaration + cursor, "#define yytext ctx->input\n");

    for (int i = 0; i < localVars->used; i++) {
        cursor += sprintf(functionDeclaration + cursor, "#define %s ctx->vars->data[%d]\n",
                (char*)getProperty(localVars->data[i], "string"), i);
    }

    cursor += sprintf(functionDeclaration + cursor, "\nvoid %s(Context *ctx) {\n\t%s\n}\n\n",
            functionName, rawActionContent);

    for (int i = 0; i < localVars->used; i++) {
        cursor += sprintf(functionDeclaration + cursor, "#undef %s\n",
                (char*)getProperty(localVars->data[i], "string"));
    }

    cursor += sprintf(functionDeclaration + cursor, "#undef $$\n");
    cursor += sprintf(functionDeclaration + cursor, "#undef yytext\n");

    cursor += sprintf(functionDeclaration + cursor, "\n");
    appendToActionSection(fileWriter, functionDeclaration);

    free(functionDeclaration);

    char *convertedActionContent = convertSpecialChars(rawActionContent);

    char *outputString = malloc(
            sizeof(char) * (64 + strlen(fullVariableName) + strlen(convertedActionContent)));
    sprintf(outputString, "oop %s = newAction(\"%s\", %s);\n",
            fullVariableName, convertedActionContent, functionName);
    appendToDeclarationSection(fileWriter, outputString);

    free(convertedActionContent);

    return fullVariableName;
}

long applyAction(oop action, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    // printf("\nExecuting Action %s\n", get(rule, Action, value));
    // printf("Current input (yytext) is %s\n", ctx->input);
    ((void (*)(Context*))getProperty(action, "function"))(ctx);
    // printf("Return value ($$) is %p\n", ctx->returnValue);
    return 1;
}

Dict *getActionMethods() {
    Dict *methods = newDict();
    set(methods, "print", printStringLeaf);
    set(methods, "write", writeAction);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyAction);
    return methods;
}

/// Identifier Class

oop newIdentifier(char *value) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop identifier = newObject("Identifier");
    setProperty(identifier, "value", newValue);
    return identifier;
}

long applyIdentifier(oop identifier, List *ruleNames, List *rules, ReadState *state, Context *ctx) {
    oop s = newSymbol(getProperty(identifier, "value"));
    int ruleIndex = indexOfByValue(ruleNames, s);

    push(state->callStack, s);

    oop innerRule = rules->data[ruleIndex];
    Context *innerContext = newContext(ctx->input);

    getMethod(innerRule, "fillContext")(innerRule, innerContext);

    // printf("Applying the following inner rule\n");
    // printExpression(innerRule, 0);

    int cursor = state->cursor;
    int lastBegin = state->lastBegin;

    long success = (long)getMethod(innerRule, "applyRule")(innerRule, ruleNames, rules, state, ctx);

    if (!success) {
        state->cursor = cursor;
        state->lastBegin = lastBegin;
    }

    state->callStack->used--;
    free(s);

    return success;
}

Dict *getIdentifierMethods() {
    Dict *methods = newDict();
    set(methods, "print", printStringLeaf);
    set(methods, "write", writeStringLeaf);
    set(methods, "fillContext", noOpMethod);
    set(methods, "applyRule", applyIdentifier);
    return methods;
}

/// Symbol Class

oop newSymbol(char *string) {
    int length = strlen(string) + 1;
    char *newString = malloc(sizeof(char) * length);
    strcpy(newString, string);

    oop symbol = newObject("Symbol");
    setProperty(symbol, "string", newString);
    return symbol;
}

Dict *getSymbolMethods() {
    Dict *methods = newDict();
    return methods;
}

/// Symbol Manipulation functions

int equals(oop obj, oop other) {
    return strcmp(getProperty(obj, "string"), getProperty(other, "string")) == 0; 
}

oop addSymbolToSymbolList(List *symbolList, oop symbol) {
    int index = indexOfByValue(symbolList, symbol);
    if (index == -1) {
        push(symbolList, symbol);
        return symbol;
    } else {
        free(symbol);
        return symbolList->data[index];
    }
}

oop addNewStringToSymbolList(List *symbolList, char *string) {
    oop s = newSymbol(string);
    return addSymbolToSymbolList(symbolList, s);
}

void declareTypes() {
    initTypes();

    declareType("Grammar", getGrammarMethods());

    declareType("Definition", getDefinitionMethods());

    declareType("Assignment", getAssignmentMethods());

    declareType("Sequence", getSequenceMethods());

    declareType("Alternation", getAlternationMethods());

    declareType("Star", getStarMethods());

    declareType("Plus", getPlusMethods());

    declareType("Optional", getOptionalMethods());

    declareType("And", getAndMethods());

    declareType("Not", getNotMethods());

    declareType("Dot", getDotMethods());

    declareType("Begin", getBeginMethods());

    declareType("End", getEndMethods());

    declareType("String", getStringMethods());

    declareType("CharacterClass", getCharacterClassMethods());

    declareType("Action", getActionMethods());

    declareType("Identifier", getIdentifierMethods());

    declareType("Symbol", getSymbolMethods());
}

// Print stuff

void printTree(oop grammar) {
    printGrammar(grammar, 0);
}

/*
 * Writes the necessary C code to a file to create the grammar tree.
 *
 * @param grammar The file to write to.
 */
void writeTree(oop grammar) {

    FileWriter *fileWriter = newFileWriter("grammar_get_tree.c");
    appendToActionSection(fileWriter, "#include \"grammar_objects.h\"\n\n");

    List *definitions = getProperty(grammar, "definitions");
    int expressionCount = 0;

    appendToDeclarationSection(fileWriter, "oop getGrammar() {\n");

    appendToDeclarationSection(fileWriter, "oop grammar = newGrammar();\n");

    for (int i = 0; i < definitions->used; i++) {
        char *varName = getMethod(definitions->data[i], "write")(
                definitions->data[i], fileWriter, &expressionCount, NULL);
        appendToDeclarationSection(fileWriter, "\n");

        char *definitionPush = calloc(sizeof(char), 256);
        sprintf(definitionPush, "push((List*)getProperty(grammar, \"definitions\"), %s);\n", varName);
        appendToDeclarationSection(fileWriter, definitionPush);
        free(definitionPush);

        appendToDeclarationSection(fileWriter, "\n");
    }


    appendToDeclarationSection(fileWriter, "\t return grammar;\n");
    appendToDeclarationSection(fileWriter, "}\n\n");

    writeToFile(fileWriter);
}

/*
 * Evaluates the tree representing grammars and outputs the tree representation of the grammar represented by string.
 */
oop evaluateTree(oop grammar, char *string) {

    List *rules = newList(10);
    List *ruleNames = newList(10);

    for (int i = 0; i < ((List*)getProperty(grammar, "definitions"))->used; i++) {
        push(rules, getProperty((oop)((List*)getProperty(grammar, "definitions"))->data[i], "rule"));
        push(ruleNames, newSymbol(
                    getProperty(
                        getProperty(
                            ((List*)getProperty(grammar, "definitions"))->data[i],
                            "name"),
                        "value")
                    ));
    }

    Context *ctx = newContext(NULL);
    ReadState state = { .cursor = 0, .string = string, .lastBegin = 0, .callStack = newList(10) };

    getMethod(rules->data[0], "fillContext")(rules->data[0], ctx);

    // printf("Starting parsing with the following rule\n");
    // printExpression(rules->data[0], 0);

    push(state.callStack, ruleNames->data[0]);

    if (getMethod(rules->data[0], "applyRule")(rules->data[0], ruleNames, rules, &state, ctx)) {
        return ctx->returnValue;

    } else {
        fatal("Couldn't parse grammar\n");
        return NULL;
    }
}

/// Old code for printing call stack when applying rules

// char *getCallStackString(List *callStack) {
//
//     char *res = calloc(sizeof(char), callStack->used * 32);
//
//     for (int i = 0; i < callStack->used; i++) {
//         res = strcat(res, " > ");
//         res = strcat(res, getProperty(callStack->data[i], "string"));
//     }
//
//     return res;
// }

// int applyRule(oop rule, List *ruleNames, List *rules, ReadState *state, Context *ctx) {

    // char* nextCharacters = calloc(sizeof(char), 16);
    // nextCharacters = strncpy(nextCharacters, state->string + state->cursor, 16);
    // nextCharacters = convertSpecialChars(nextCharacters);
    //
    // char* callStackString = getCallStackString(state->callStack);

    // printf("\nParsing %s (%d)\nIn%s\n>>>>>   [%s]\n",
    //         getTypeString(rule->type), state->cursor, callStackString, nextCharacters);

    // free(callStackString);
    // free(nextCharacters);
        // TODO: Add verification of right amount of <>

// }
