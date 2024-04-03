#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

void fatal(char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "\n");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

union Object;

typedef union Object *oop;

// List

typedef struct 
{
    size_t size;
    size_t used; 
    oop data[];
} List;


List *addsize(List *list, size_t inc)
{
    size_t newsize = 0;
    size_t newused = 0;
    
    if(list) {
        newsize = list -> size + inc;
        newused = list -> used;
    }

    list = realloc(list, sizeof(*list) + newsize * sizeof(oop*));
    printf("Reallocating list\n");

    if(list) {
        list -> size = newsize;
        list -> used = newused;
    }

    return list;
}

List *push(List *list, oop object) {
    
    if (list->size == list->used) {
        List *new_list = addsize(list, 32);
        new_list->data[new_list->used] = object;
        new_list->used += 1;
        return new_list;

    } else {
        list->data[list->used] = object;
        list->used += 1;
        return list;

    }
}

List *newList(int size) {
    
    List* list = malloc(sizeof(List) + size * sizeof(oop*));
    
    list->size = size;
    list->used = 0;
    
    return list;
}

enum Types {
    Grammar,
    Binary,
    Unary,
    Dot,
    Begin,
    End,
    String,
    CharacterClass,
    Action,
    Identifier,
};

char *getTypeString(enum Types type) {
    switch (type) {
        case Grammar: return "Grammar";
        case Binary: return "Binary";
        case Unary: return "Unary";
        case Dot: return "Dot";
        case Begin: return "Begin";
        case End: return "End";
        case String: return "String";
        case CharacterClass: return "CharacterClass";
        case Action: return "Action";
        case Identifier: return "Identifier";
    }
}

struct Grammar {
    int type;
    List *definitions;
};

struct Binary {
    int type;
    int op;
    oop leftExpression;
    oop rightExpression;
};

enum BinaryOperators {
    Sequence,
    Alternation,
    Definition,
    Assignment,
};

char *getBinaryOpString(enum BinaryOperators op) {
    switch (op) {
        case Sequence: return "Sequence";
        case Alternation: return "Alternation";
        case Definition: return "Definition";
        case Assignment: return "Assignment";
    }
}

struct Unary {
    int type;
    int op;
    oop expression;
};

enum UnaryOperators {
    Star,
    Plus,
    Optional,
    And,
    Not,
};

char *getUnaryOpString(enum UnaryOperators op) {
    switch (op) {
        case Star: return "Star";
        case Plus: return "Plus";
        case Optional: return "Optional";
        case And: return "And";
        case Not: return "Not";
    }
}

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

union Object {
    int type;
    struct Grammar Grammar;
    struct Binary Binary;
    struct Unary Unary;
    struct Dot Dot;
    struct Begin Begin;
    struct End End;
    struct String String;
    struct CharacterClass CharacterClass;
    struct Action Action;
    struct Identifier Identifier;
};

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

void addRuleDefinitionToGrammar(oop grammar, oop definition) {
    printf("Adding Rule to grammar\n");
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

// Print stuff

void printExpression(oop expression, int depth) {
    switch (expression->type) {
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
    char *convertedString = malloc(sizeof(char) * strlen(sourceString));
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
        case Binary: {

            char *leftExpressionVarName = writeExpression(fptr, get(expression, Binary, leftExpression), declarationCount);
            char *rightExpressionVarName = writeExpression(fptr, get(expression, Binary, rightExpression), declarationCount);
            
            enum BinaryOperators op = get(expression, Binary, op);
            char *variableName = getBinaryOpString(op);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            // Do I need to free variableName after this ???

            char *operatorName = getBinaryOpString(op);
            fprintf(fptr, "\toop %s = newBinary(%s, %s, %s);\n", 
                    fullVariableName, operatorName, leftExpressionVarName, rightExpressionVarName);


            return fullVariableName;
        }
        case Unary: {
            
            char *expressionVarName = writeExpression(fptr, get(expression, Unary, expression), declarationCount);

            enum UnaryOperators op = get(expression, Unary, op);
            char *variableName = getUnaryOpString(op);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            char *operatorName = getUnaryOpString(op);
            fprintf(fptr, "\toop %s = newUnary(%s, %s);\n", fullVariableName, operatorName, expressionVarName);

            return fullVariableName;
        }
        case Dot: {
            
            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "\toop %s = newDot();\n", fullVariableName);

            return fullVariableName;
        }

        case Begin: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "\toop %s = newBegin();\n", fullVariableName);

            return fullVariableName;
        }

        case End: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            fprintf(fptr, "\toop %s = newEnd();\n", fullVariableName);

            return fullVariableName;
        }

        case String: {

            char *variableName = getTypeString(expression->type);

            (*declarationCount)++;

            char *fullVariableName = malloc(sizeof(char) * 64);
            sprintf(fullVariableName, "%s%d", variableName, *declarationCount);

            char *value = get(expression, String, value);

            char *convertedValue = convertSpecialChars(value);
            fprintf(fptr, "\toop %s = newString(\"%s\");\n", fullVariableName, convertedValue);
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
            fprintf(fptr, "\toop %s = newCharacterClass(\"%s\");\n", fullVariableName, convertedValue);
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
            fprintf(fptr, "\toop %s = newAction(\"%s\");\n", fullVariableName, convertedValue);
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
            fprintf(fptr, "\toop %s = newIdentifier(\"%s\");\n", fullVariableName, convertedValue);
            free(convertedValue);

            return fullVariableName;
        }
        default: fatal("shit hit the fan in write tree"); return "";
    }
}

void writeTree(oop grammar) {
    FILE *fptr = fopen("grammar_tree_out.c", "w");

    fprintf(fptr, "#include \"grammar_objects.h\"\n\noop getGrammar() {\n");
    fprintf(fptr, "\toop grammar = newGrammar();\n");

    List *definitions = get(grammar, Grammar, definitions);
    int expressionCount = 0;

    for (int i = 0; i < definitions->used; i++) {
        char *varName = writeExpression(fptr, definitions->data[i], &expressionCount);
        fprintf(fptr, "\taddRuleDefinitionToGrammar(grammar, %s);\n\n", varName);
    }

    fprintf(fptr, "\treturn grammar;\n}\n");

    fclose(fptr);
}
