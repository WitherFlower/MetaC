#include "grammar_objects.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <strings.h>
#include <regex.h>

#include "list.h"
#include "wonkyregex.h"

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
    // printf("Creating new Binary with operator %s, expressions %p and %p\n", getBinaryOpString(op), leftExpression, rightExpression);
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
    // printf("Creating new String with value %s\n", value);
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

oop newAction(char *value, void (*function)()) {
    int length = strlen(value) + 1;
    char *newValue = malloc(sizeof(char) * length);
    strcpy(newValue, value);

    oop action = newObject(Action);
    set(action, Action, value, newValue);

    set(action, Action, function, function);
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

oop addSymbolToSymbolList(List *symbolList, oop symbol) {
    int index = indexOfByValue(symbolList, symbol);
    if (index == -1) {
        *symbolList = *push(symbolList, symbol);
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

/*
 * Writes the necessary C code to a file to create the expression,
 * and returns the name give to the instantiated variable.
 *
 * @param fptr The file to write to.
 * @param expression The expression to write.
 * @param declarationCount pointer to the number of variables declared so far
 * @param localVars pointer to a list of the current variables in the definition
 */
char *writeExpression(FILE *fptr, oop expression, int *declarationCount, List *localVars) {
    switch (expression->type) {
        case Definition: {
            
            char *definitionName = writeExpression(fptr, get(expression, Definition, name), declarationCount, localVars);

            List *l = newList(10);
            char *ruleVarName = writeExpression(fptr, get(expression, Definition, rule), declarationCount, l);

            for (int i = 0; i < l->used; i++) {
                free(l->data[i]);
            }

            free(l);

            const char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .Definition.type = Definition, .Definition.name = &%s, .Definition.rule = &%s };\n",
                    fullVariableName, definitionName, ruleVarName);

            return fullVariableName;
        }
        case Assignment: {
            
            char *assignedVariableName = writeExpression(fptr, get(expression, Assignment, variableName), declarationCount, localVars);

            addNewStringToSymbolList(localVars, get(get(expression, Assignment, variableName), String, value));
            char *ruleIdentifierName = writeExpression(fptr, get(expression, Assignment, ruleIdentifier), declarationCount, localVars);

            const char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "union Object %s = { .Assignment.type = Assignment, .Assignment.variableName = &%s, .Assignment.ruleIdentifier = &%s };\n",
                    fullVariableName, assignedVariableName, ruleIdentifierName);

            return fullVariableName;
        }
        case Binary: {

            char *leftExpressionVarName = writeExpression(fptr, get(expression, Binary, leftExpression), declarationCount, localVars);
            char *rightExpressionVarName = writeExpression(fptr, get(expression, Binary, rightExpression), declarationCount, localVars);
            
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
            
            char *expressionVarName = writeExpression(fptr, get(expression, Unary, expression), declarationCount, localVars);

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

            char *functionName = malloc(sizeof(char) * 64);
            sprintf(functionName, "%s%d_function", variableName, *declarationCount);

            char *rawActionContent = get(expression, Action, value);

            fprintf(fptr, "\n");
            fprintf(fptr, "#define $$ ctx->returnValue\n");
            fprintf(fptr, "#define yytext ctx->input\n");

            for (int i = 0; i < localVars->used; i++) {
                fprintf(fptr, "#define %s ctx->vars->data[%d]\n", get(localVars->data[i], Symbol, string), i);
            }

            fprintf(fptr, "\nvoid %s(Context *ctx) {\n\t%s\n}\n\n", functionName, rawActionContent);

            for (int i = 0; i < localVars->used; i++) {
                fprintf(fptr, "#undef %s\n", get(localVars->data[i], Symbol, string));
            }

            fprintf(fptr, "#undef $$\n");
            fprintf(fptr, "#undef yytext\n");

            fprintf(fptr, "\n");

            char *convertedActionContent = convertSpecialChars(rawActionContent);
            fprintf(fptr, "union Object %s = { .Action.type = Action, .Action.value = \"%s\", .Action.function = %s };\n",
                    fullVariableName, convertedActionContent, functionName);
            free(convertedActionContent);

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
        char *varName = writeExpression(fptr, definitions->data[i], &expressionCount, NULL);
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

Context *newContext(char *currentInput) {
    Context *ctx = malloc(sizeof(Context));
    ctx->varNames = newList(10);
    ctx->vars = newList(10);
    ctx->input = currentInput;
    ctx->returnValue = NULL;
    return ctx;
}

void fillContextWithAssignments(Context *ctx, oop rule) {
    switch (rule->type) {
        case Assignment:
            // printf("Adding variable %s to context\n", get(get(rule, Assignment, variableName), String, value));
            addNewStringToSymbolList(ctx->varNames, get(get(rule, Assignment, variableName), String, value));
            break;

        case Binary:
            fillContextWithAssignments(ctx, get(rule, Binary, leftExpression));
            fillContextWithAssignments(ctx, get(rule, Binary, rightExpression));
            break;

        case Unary:
            fillContextWithAssignments(ctx, get(rule, Unary, expression));
            break;

        default:
            break;
    }
}

char *getCallStackString(List *callStack) {

    char *res = calloc(sizeof(char), callStack->used * 32);

    for (int i = 0; i < callStack->used; i++) {
        res = strcat(res, " > ");
        res = strcat(res, get(callStack->data[i], Symbol, string));
    }

    return res;
}

int applyRule(oop rule, List *ruleNames, List *rules, ReadState *state, Context *ctx) {

    char* nextCharacters = calloc(sizeof(char), 16);
    nextCharacters = strncpy(nextCharacters, state->string + state->cursor, 16);
    nextCharacters = convertSpecialChars(nextCharacters);

    char* callStackString = getCallStackString(state->callStack);

    // printf("\nParsing %s (%d)\nIn%s\n>>>>>   [%s]\n",
    //         getTypeString(rule->type), state->cursor, callStackString, nextCharacters);

    free(callStackString);
    free(nextCharacters);

    switch (rule->type) {

        case Definition:
            fatal("Tried to apply a rule of type Definition, but doesn't make sense\n");
            return 0;

        case Assignment: {
            oop s = newSymbol(get(get(rule, Assignment, ruleIdentifier), Identifier, value));
            int ruleIndex = indexOfByValue(ruleNames, s);

            oop innerRule = rules->data[ruleIndex];
            Context *innerContext = newContext(ctx->input);

            // printf("Applying the following inner rule\n");
            // printExpression(innerRule, 0);

            state->callStack = push(state->callStack, s);

            int cursor = state->cursor;
            int lastBegin = state->lastBegin;

            fillContextWithAssignments(innerContext, innerRule);

            int success = applyRule(innerRule, ruleNames, rules, state, innerContext);

            if (success) {
                oop varSymbol = newSymbol(get(get(rule, Assignment, variableName), String, value));

                // printf("Successfully parsed assignment to variable %s\n", get(varSymbol, Symbol, string));
                // printf("Value :\n");
                // printExpression(innerContext->returnValue, 0);
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

        case Binary: {
            // printf("\tParsing %s (%d)\n", getBinaryOpString(get(rule, Binary, op)), state->cursor);
            switch (get(rule, Binary, op)) {
                case Sequence:
                    if (applyRule(get(rule, Binary, leftExpression), ruleNames, rules, state, ctx)) {
                        return applyRule(get(rule, Binary, rightExpression), ruleNames, rules, state, ctx);
                    }
                    return 0;
                case Alternation: {

                    int cursor = state->cursor;
                    int lastBegin = state->lastBegin;

                    if (!applyRule(get(rule, Binary, leftExpression), ruleNames, rules, state, ctx)) {
                        // printf("Going to next alternation choice\n");
                        state->cursor = cursor;
                        state->lastBegin = lastBegin;
                        return applyRule(get(rule, Binary, rightExpression), ruleNames, rules, state, ctx);
                    }
                    return 1;
                }
            }
        }

        case Unary: {
            // printf("\tParsing %s (%d)\n", getUnaryOpString(get(rule, Unary, op)), state->cursor);
            switch (get(rule, Unary, op)) {
                case Star:
                    while (applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx));
                    return 1;

                case Plus:
                    if (!applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx))
                        return 0;

                    while (applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx));
                    return 1;

                case Optional:
                    applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx);
                    return 1;

                // Not sure what to do here
                case And: {
                    int cursor = state->cursor;
                    if (applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx)) {
                        state->cursor = cursor;
                        return applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx);
                    }
                }
                
                case Not: {
                    int cursor = state->cursor;
                    // printf("Applying Not with cursor = %d\n", cursor);

                    if (applyRule(get(rule, Unary, expression), ruleNames, rules, state, ctx)) {
                        // printf("Exiting Not\n");
                        state->cursor = cursor;
                        return 0;

                    } else {
                        return 1;
                    }
                }
            }
        }

        case Dot:
            if (state->string[state->cursor] != '\0') {
                state->cursor++;
                return 1;
            }
            return 0;

        // TODO: Add verification of right amount of <>
        case Begin:
            state->lastBegin = state->cursor;
            return 1;

        case End:
            if (ctx->input != NULL) {
                free(ctx->input);
            }
            unsigned int inputSize = state->cursor - state->lastBegin;
            ctx->input = calloc(sizeof(char), state->cursor - state->lastBegin + 1);
            strncpy(ctx->input, state->string + state->lastBegin, inputSize);
            return 1;

        case String: {

            int n = strlen(get(rule, String, value));
            char *received = calloc(1 + n, sizeof(char));
            strncpy(received, state->string + state->cursor, n);
            received = convertSpecialChars(received);
            received[n] = '\0';

            // printf("Received String : \"%s\"\n", received);

            char *expected = get(rule, String, value);
            // printf("Expected String : \"%s\"\n", get(rule, String, value));

            if (strcmp(received, expected) == 0) {
                state->cursor += getConvertedStringRealLength(get(rule, String, value));
                // printf("Equal, incrementing cursor by %u\n", getConvertedStringRealLength(get(rule, String, value)));
                free(received);
                return 1;
            }
            // printf("Not Equal\n");
            free(received);
            return 0;
        }

        case CharacterClass: {

            char *classRegex = malloc(sizeof(char) * (3 + strlen(get(rule, CharacterClass, value))));
            sprintf(classRegex, "[%s]", get(rule, CharacterClass, value));

            // printf("classRegex value : %s\n", classRegex);
            
            char nextChar[] = { state->string[state->cursor], '\0' };

            // printf("nextChar: %c\n", nextChar[0]);

            if (match(classRegex, nextChar)) {
                state->cursor++;
                return 1;
            }

            return 0;
        }

        case Action:
            // printf("\nExecuting Action %s\n", get(rule, Action, value));
            // printf("Current input (yytext) is %s\n", ctx->input);
            get(rule, Action, function)(ctx);
            // printf("Return value ($$) is %p\n", ctx->returnValue);
            return 1;

        case Identifier: {
            oop s = newSymbol(get(rule, Identifier, value));
            int ruleIndex = indexOfByValue(ruleNames, s);

            state->callStack = push(state->callStack, s);

            oop innerRule = rules->data[ruleIndex];
            Context *innerContext = newContext(ctx->input);

            fillContextWithAssignments(innerContext, innerRule);

            // printf("Applying the following inner rule\n");
            // printExpression(innerRule, 0);

            int cursor = state->cursor;
            int lastBegin = state->lastBegin;

            int success = applyRule(innerRule, ruleNames, rules, state, ctx);

            if (!success) {
                state->cursor = cursor;
                state->lastBegin = lastBegin;
            }

            state->callStack->used--;
            free(s);

            return success;
        }
            
        default:
            fatal("Shit hit the fan\n");
            return 0;
    }
}

/*
 * Evaluates the tree representing grammars and outputs the tree representation of the grammar represented by string.
 */
oop evaluateTree(oop grammar, char *string) {

    List *rules = newList(10);
    List *ruleNames = newList(10);

    for (int i = 0; i < get(grammar, Grammar, definitions)->used; i++) {
        rules = push(rules, get(get(grammar, Grammar, definitions)->data[i], Definition, rule));
        ruleNames = push(ruleNames, newSymbol(get(get(get(grammar, Grammar, definitions)->data[i], Definition, name), String, value)));
    }

    Context *ctx = newContext(NULL);
    ReadState state = { .cursor = 0, .string = string, .lastBegin = 0, .callStack = newList(10) };

    fillContextWithAssignments(ctx, rules->data[0]);

    // printf("Starting parsing with the following rule\n");
    // printExpression(rules->data[0], 0);

    state.callStack = push(state.callStack, ruleNames->data[0]);

    if (applyRule(rules->data[0], ruleNames, rules, &state, ctx)) {
        return ctx->returnValue;

    } else {
        fatal("Couldn't parse grammar\n");
        return NULL;
    }
}

