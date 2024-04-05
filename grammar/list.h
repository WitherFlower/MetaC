#include <stdlib.h>
#include "grammar_objects.h"

#ifndef LIST_H
#define LIST_H

List *newList(int size);

List *addsize(List *list, size_t inc);

List *push(List *list, oop object);

List *insertAt(List *list, unsigned int index, oop object);

/*
 * Finds the index of a given object in the list
 * Implies the list is sorted by pointer value
 */
int indexOf(List *list, oop object); 

List *insertSorted(List *list, oop object);

int indexOfByValue(List *list, oop object);

#endif
