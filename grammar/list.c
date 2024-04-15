#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "grammar_objects.h"

List *newList(int size) {
    
    List* list = malloc(sizeof(List));

    list->size = size;
    list->used = 0;
    list->data = calloc(sizeof(void*), size);
    
    return list;
}

void addsize(List *list, size_t inc) {
    size_t newsize = 0;
    size_t newused = 0;
    
    if(list) {
        newsize = list -> size + inc;
        newused = list -> used;
    }

    list->data = realloc(list->data, newsize * sizeof(void*));

    if(list->data) {
        list -> size = newsize;
        list -> used = newused;
    }
}

void push(List *list, void *object) {
    
    if (list->size == list->used) {
        addsize(list, 32);
        list->data[list->used] = object;
        list->used += 1;

    } else {
        list->data[list->used] = object;
        list->used += 1;

    }
}

void insertAt(List *list, unsigned int index, void *object) {
    
    if (list->size == list->used) {
        addsize(list, 32);
    }

    memmove(list->data + index + 1, list->data + index, (list->used - index) * sizeof(void*));

    list->data[index] = object;
    list->used += 1;
}

/*
 * Finds the index of a given object in the list
 *
 * Implies the list is sorted by pointer value
 */
int indexOf(List *list, void *object) {

    if (list->used == 0) return -1;

    int low = 0;
    int high = list->used-1;

    // Happens when list is empty
    if (high < 0) {
        high = 0;
    }

    while (high - low > 0) {

        int mid = (low + high) / 2;

        if (list->data[mid] > object) {
            high = mid;

        } else if (list->data[mid] < object) {
            low = mid + 1;

        } else {
            assert(list->data[mid] == object);
            return mid;
        }

        assert(high >= low);
    }

    // printf("used: %lu\n", list->used);
    // printf("hi: %d, lo: %d\n", high, low);
    assert(high == low);

    if (list->data[low] != object) {
        return -1;
    }

    return low;
}

void insertSorted(List *list, void *object) {

    if (list->used == 0) {
        return push(list, object);
    }

    int low = 0;
    int high = list->used-1;

    // Happens when list is empty
    if (high < 0) {
        high = 0;
    }

    while (high - low > 0) {

        int mid = (low + high) / 2;

        if (list->data[mid] > object) {
            high = mid;

        } else if (list->data[mid] < object) {
            low = mid + 1;

        } else {
            assert(list->data[mid] == object);
            return insertAt(list, mid, object);
        }
        
        // printf("data[mid] = %lu\n", list->data[mid]);
        // printf("hi: %d, lo: %d\n", high, low);
        assert(high >= low);
    }

    // printf("end of insertSorted\n");
    // printf("\tused (before call): %lu\n", list->used);
    // printf("\thi: %d, lo: %d\n\n", high, low);
    assert(high == low);

    if (list->data[low] < object) {
        return insertAt(list, low + 1, object);
    } else {
        return insertAt(list, low, object);
    }
}

int indexOfByValue(List *list, void *object) {
    
    for (int i = 0; i < list->used; i++) {
        if (equals(object, list->data[i])) {
            return i;
        }
    }

    return -1;
}
