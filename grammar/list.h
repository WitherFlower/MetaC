#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

typedef struct 
{
    size_t size;
    size_t used; 
    void **data;
} List;

List *newList(int size);

void addsize(List *list, size_t inc);

void push(List *list, void *object);

void insertAt(List *list, unsigned int index, void *object);

/*
 * Finds the index of a given object in the list
 * Implies the list is sorted by pointer value
 */
int indexOf(List *list, void *object); 

void insertSorted(List *list, void *object);

int indexOfByValue(List *list, void *object);

#endif
