#ifndef DICT_H
#define DICT_H

#include "list.h"

typedef struct Dict {
    List *pairs;
} Dict;

typedef struct Pair {
    char *key;
    void *value;
} Pair;

Pair *newPair(char* key, void *value);

Dict *newDict();

void set(Dict *dict, char *key, void *value); 

void *get(Dict *dict, char *key); 

#endif

