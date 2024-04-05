#include "grammar_objects.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <strings.h>
#include "list.h"

void fatal(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "\n");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

char *getTypeString(enum Types type) {
    switch (type) {
        case Grammar: return "Grammar";
        case Definition: return "Definition";
        case Assignment: return "Assignment";
        case Binary: return "Binary";
        case Unary: return "Unary";
        case Dot: return "Dot";
        case Begin: return "Begin";
        case End: return "End";
        case String: return "String";
        case CharacterClass: return "CharacterClass";
        case Action: return "Action";
        case Identifier: return "Identifier";
        case Symbol: return "Symbol";
    }
}

const char *getBinaryOpString(enum BinaryOperators op) {
    switch (op) {
        case Sequence: return "Sequence";
        case Alternation: return "Alternation";
    }
}

const char *getUnaryOpString(enum UnaryOperators op) {
    switch (op) {
        case Star: return "Star";
        case Plus: return "Plus";
        case Optional: return "Optional";
        case And: return "And";
        case Not: return "Not";
    }
}

oop _newObject(enum Types type, size_t size) {
    oop this = malloc(size);
    this->type = type;
    return this;
}

oop _checkType(oop object, enum Types type, char *file, int lineNumber) {
    if (object->type != type)
        fatal("[%s:%d] Expected type %s, got %s\n", file, lineNumber, getTypeString(type), getTypeString(object->type));
    return object;
}

#define newObject(TYPE) _newObject(TYPE, sizeof (struct TYPE))
#define get(VAL, TYPE, FIELD) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD
#define set(VAL, TYPE, FIELD, NEW_FIELD_VALUE) _checkType(VAL, TYPE, __FILE__, __LINE__)->TYPE.FIELD=NEW_FIELD_VALUE

oop newGrammar() {
    oop grammar = newObject(Grammar);
    set(grammar, Grammar, definitions, newList(10));
    return grammar;
}

oop newDefinition(oop name, oop rule) {

    oop definition = newObject(Definition);
    set(definition, Definition, name, name);
    set(definition, Definition, rule, rule);

    return definition;
}

oop newAssignment(oop variableName, oop ruleIdentifier) {

    oop assignment = newObject(Assignment);
    set(assignment, Assignment, variableName, variableName);
    set(assignment, Assignment, ruleIdentifier, ruleIdentifier);

    return assignment;
}

void addRuleDefinitionToGrammar(oop grammar, oop definition) {
    // printf("Adding Rule to grammar\n");
    List *rootExpressions = get(grammar, Grammar, definitions);
    set(grammar, Grammar, definitions, push(rootExpressions, definition));
}

oop newBinary(enum BinaryOperators op, oop leftExpression, oop rightExpression) {
    oop binary = newObject(Binary);
    set(binary, Binary, op, op);
    set(binary, Binary, leftExpression, leftExpression);
    set(binary, Binary, rightExpression, rightExpression);
    return binary;
}

oop newUnary(enum UnaryOperators op, oop expression) {
    oop unary = newObject(Unary);
    set(unary, Unary, op, op);
    set(unary, Unary, expression, expression);
    return unary;
}

oop newDot() {
    return newObject(Dot);
}

oop newBegin() {
    return newObject(Begin);
}

oop newEnd() {
    return newObject(End);
}

oop newString(char *value) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop string = newObject(String);
    set(string, String, value, newValue);
    return string;
}

oop newCharacterClass(char *value) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop characterClass = newObject(CharacterClass);
    set(characterClass, CharacterClass, value, newValue);
    return characterClass;
}

oop newAction(char *value) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop action = newObject(Action);
    set(action, Action, value, newValue);
    return action;
}

oop newIdentifier(char *value) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop identifier = newObject(Identifier);
    set(identifier, Identifier, value, newValue);
    return identifier;
}

oop newSymbol(char *string) {
    int length = strlen(string) + 1;
    char *newString = malloc(sizeof(char) * length);
    strcpy(newString, string);

    oop symbol = newObject(Symbol);
    set(symbol, Symbol, string, newString);
    return symbol;
}

oop addNewSymbolToList(List *symbolList, char *string) {
    oop s = newSymbol(string);
    int index = indexOfByValue(symbolList, s);
    if (index == -1) {
        *symbolList = *insertSorted(symbolList, s);
        return s;
    } else {
        free(s);
        return symbolList->data[index];
    }
}

int objectEquals(oop obj, oop other) {
    if (obj->type != other->type) {
        return 0;
    }

    switch (obj->type) {
        case Symbol: return strcmp(get(obj, Symbol, string), get(other, Symbol, string)) == 0; 
        default: fatal("Equality unimplemented for type %s", getTypeString(obj->type)); return 0;
    }
}

// Print stuff

void printExpression(oop expression, int depth) {
    switch (expression->type) {
        case Definition:
            printf("%*s| %s (%s)\n", 2 * depth, "", getTypeString(Definition),
                    get(get(expression, Definition, name), String, value));
            printExpression(get(expression, Definition, rule), depth + 1);
            break;

        case Assignment:
            printf("%*s| %s (%s)\n", 2 * depth, "", getTypeString(Assignment),
                    get(get(expression, Assignment, variableName), String, value));
            printExpression(get(expression, Assignment, ruleIdentifier), depth + 1);
            break;

        case Binary:
            printf(
                "%*s| %s (%s)\n",
                2 * depth,
                "",
                getTypeString(expression->type),
                getBinaryOpString(get(expression, Binary, op))
            );

            printExpression(get(expression, Binary, leftExpression), depth + 1);
            printExpression(get(expression, Binary, rightExpression), depth + 1);

            break;

        case Unary:
            printf(
                "%*s| %s (%s)\n",
                2 * depth,
                "",
                getTypeString(expression->type),
                getUnaryOpString(get(expression, Unary, op))
            );

            printExpression(get(expression, Unary, expression), depth + 1);

            break;

        case Dot:
            printf("%*s| %s\n", 2 * depth, "", getTypeString(Dot));
            break;

        case Begin:
            printf("%*s| %s\n", 2 * depth, "", getTypeString(Begin));
            break;

        case End:
            printf("%*s| %s\n", 2 * depth, "", getTypeString(End));
            break;

        case String:
            printf("%*s| %s (%s)\n", 2 * depth, "", getTypeString(String), get(expression, String, value));
            break;

        case CharacterClass:
            printf("%*s| %s (%s)\n", 2 * depth, "", getTypeString(CharacterClass), get(expression, CharacterClass, value));
            break;

        case Action:
            printf("%*s| %s (%s)\n", 2 * depth, "", getTypeString(Action), get(expression, Action, value));
            break;

        case Identifier:
            printf("%*s| %s (%s)\n", 2 * depth, "", getTypeString(Identifier), get(expression, Identifier, value));
            break;
    }
}

void printTree(oop grammar) {
    List *definitions = get(grammar, Grammar, definitions);
    for (int i = 0; i < definitions->used; i++) {
        printExpression(definitions->data[i], 0);
    }
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

/*
 * Writes the necessary C code to a file to create the expression,
 * and returns the name give to the instantiated variable.
 *
 * @param fptr The file to write to.
 * @param expression The expression to write.
 */
char *writeExpression(FILE *fptr, oop expression, int *declarationCount) {
    switch (expression->type) {
        case Definition: {
            
            char *definitionName = writeExpression(fptr, get(expression, Definition, name), declarationCount);
            char *ruleVarName = writeExpression(fptr, get(expression, Definition, rule), declarationCount);

            const char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .Definition.type = Definition, .Definition.name = &%s, .Definition.rule = &%s };\n",
                    fullVariableName, definitionName, ruleVarName);

            return fullVariableName;
        }
        case Assignment: {
            
            char *assignedVariableName = writeExpression(fptr, get(expression, Assignment, variableName), declarationCount);
            char *ruleIdentifierName = writeExpression(fptr, get(expression, Assignment, ruleIdentifier), declarationCount);

            const char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .Assignment.type = Assignment, .Assignment.variableName = &%s, .Assignment.ruleIdentifier = &%s };\n",
                    fullVariableName, assignedVariableName, ruleIdentifierName);

            return fullVariableName;
        }
        case Binary: {

            char *leftExpressionVarName = writeExpression(fptr, get(expression, Binary, leftExpression), declarationCount);
            char *rightExpressionVarName = writeExpression(fptr, get(expression, Binary, rightExpression), declarationCount);
            
            enum BinaryOperators op = get(expression, Binary, op);
            const char *variableName = getBinaryOpString(op);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            const char *operatorName = getBinaryOpString(op);
            fprintf(fptr, "union Object %s = { .Binary.type = Binary, .Binary.op = %s, .Binary.leftExpression = &%s, .Binary.rightExpression = &%s };\n",
                    fullVariableName, operatorName, leftExpressionVarName, rightExpressionVarName);


            return fullVariableName;
        }
        case Unary: {
            
            char *expressionVarName = writeExpression(fptr, get(expression, Unary, expression), declarationCount);

            enum UnaryOperators op = get(expression, Unary, op);
            const char *variableName = getUnaryOpString(op);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            const char *operatorName = getUnaryOpString(op);
            fprintf(fptr, "union Object %s = { .Unary.type = Unary, .Unary.op = %s, .Unary.expression = &%s };\n",
                    fullVariableName, operatorName, expressionVarName);

            return fullVariableName;
        }
        case Dot: {
            
            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .Dot.type = Dot };\n", fullVariableName);

            return fullVariableName;
        }

        case Begin: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .Begin.type = Begin };\n", fullVariableName);

            return fullVariableName;
        }

        case End: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .End.type = End };\n", fullVariableName);

            return fullVariableName;
        }

        case String: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            char *value = get(expression, String, value);

            char *convertedValue = convertSpecialChars(value);
            fprintf(fptr, "union Object %s = { .String.type = String, .String.value = \"%s\" };\n",
                    fullVariableName, convertedValue);
            free(convertedValue);

            return fullVariableName;
        }

        case CharacterClass: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            char *value = get(expression, CharacterClass, value);

            char *convertedValue = convertSpecialChars(value);
            fprintf(fptr, "union Object %s = { .CharacterClass.type = CharacterClass, .CharacterClass.value = \"%s\" };\n",
                    fullVariableName, convertedValue);
            free(convertedValue);

            return fullVariableName;
        }

        case Action: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            char *value = get(expression, Action, value);

            char *convertedValue = convertSpecialChars(value);
            fprintf(fptr, "union Object %s = { .Action.type = Action, .Action.value = \"%s\" };\n",
                    fullVariableName, convertedValue);
            free(convertedValue);

            return fullVariableName;
        }

        case Identifier: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            char *value = get(expression, Identifier, value);

            char *convertedValue = convertSpecialChars(value);
            fprintf(fptr, "union Object %s = { .Identifier.type = Identifier, .Identifier.value = \"%s\" };\n",
                    fullVariableName, convertedValue);
            free(convertedValue);

            return fullVariableName;
        }
        default: fatal("shit hit the fan in write tree"); return "";
    }
}

void writeTree(oop grammar) {
    FILE *fptr = fopen("grammar_get_tree.c", "w");

    fprintf(fptr, "#include \"grammar_objects.h\"\n\n");

    List *definitions = get(grammar, Grammar, definitions);
    int expressionCount = 0;

    List *definitionVariableNames = newList(10);

    for (int i = 0; i < definitions->used; i++) {
        char *varName = writeExpression(fptr, definitions->data[i], &expressionCount);
        fprintf(fptr, "\n");
        definitionVariableNames = push(definitionVariableNames, newString(varName));
    }

    fprintf(fptr, "List definitions = { .size = %lu, .used = %lu, .data = { ",
            definitionVariableNames->size, definitionVariableNames->used); 

    for (int i = 0; i < definitionVariableNames->used; i++) {
        fprintf(fptr, "&%s, ", get(definitionVariableNames->data[i], String, value));
    }

    fprintf(fptr, "} };\n\n");

    fprintf(fptr, "union Object grammar = { .Grammar.type = Grammar, .Grammar.definitions = &definitions };\n\n");

    fprintf(fptr, "oop getGrammar() {\n");
    fprintf(fptr, "\t return &grammar;\n");
    fprintf(fptr, "}\n\n");

    fclose(fptr);
}



/*
 * Evaluates the tree representing grammars and outputs the tree representation of the grammar represented by string.
 */
oop evaluateTree(oop grammar, char *string) {
}

