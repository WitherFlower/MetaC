#include "dict.h"

#include <stdio.h>
#include <string.h>

#include "list.h"

Pair *newPair(char* key, void *value) {
    Pair *p = malloc(sizeof(Pair));
    p->key = key;
    p->value = value;
    return p;
}

Dict *newDict() {
    Dict *d = malloc(sizeof(Dict));
    d->pairs = newList(10);
    return d;
}

void set(Dict *dict, char *key, void *value) {
    for (int i = 0; i < dict->pairs->used; i++) {

        if (strcmp(key, ((Pair*)dict->pairs->data[i])->key) == 0) {
            ((Pair*)dict->pairs->data[i])->value = value;
            return;
        }

    }
    Pair *p = newPair(key, value);
    push(dict->pairs, p);
}

void *get(Dict *dict, char *key) {
    for (int i = 0; i < dict->pairs->used; i++) {

        if (strcmp(key, ((Pair*)dict->pairs->data[i])->key) == 0) {
            return ((Pair*)dict->pairs->data[i])->value;
        }
    }
    // printf("Could not find value with key %s", key);
    return NULL;
}

