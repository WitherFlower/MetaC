#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "grammar_objects.h"

List *newList(int size) {
    
    List* list = malloc(sizeof(List) + size * sizeof(oop*));

    list->size = size;
    list->used = 0;
    
    return list;
}

List *addsize(List *list, size_t inc) {
    size_t newsize = 0;
    size_t newused = 0;
    
    if(list) {
        newsize = list -> size + inc;
        newused = list -> used;
    }

    list = realloc(list, sizeof(*list) + newsize * sizeof(oop*));

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

List *insertAt(List *list, unsigned int index, oop object) {
    
    if (list->size == list->used) {
        list = addsize(list, 32);
    }

    memmove(list->data + index + 1, list->data + index, (list->used - index) * sizeof(oop));

    list->data[index] = object;
    list->used += 1;

    return list;

}

/*
 * Finds the index of a given object in the list
 *
 * Implies the list is sorted by pointer value
 */
int indexOf(List *list, oop object) {

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

List *insertSorted(List *list, oop object) {

    if (list->used == 0) return push(list, object);

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

int indexOfByValue(List *list, oop object) {
    
    for (int i = 0; i < list->used; i++) {
        if (objectEquals(object, list->data[i])) {
            return i;
        }
    }

    return -1;
}
