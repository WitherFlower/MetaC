#include <assert.h>
#include <stdio.h>
#include "list.h"
#include "grammar_objects.h"

void printList(List *l) {
    for (int i = 0; i < l->used; i++) {
        printf("%lu ", l->data[i]);
    }
    printf("\n");
}

int main() {
    List *l = newList(10);

    assert(indexOf(l, (oop)0) == -1);
    assert(indexOf(l, (oop)1) == -1);

    push(l, (oop)1);

    assert(indexOf(l, (oop)1) == 0);
    assert(indexOf(l, (oop)2) == -1);

    for (long i = 10; i < 20; i += 2) {
        printList(l);
        assert(indexOf(l, (oop)i) == -1);
        printf("Inserting %ld\n", i);
        l = insertSorted(l, (oop)i);
    }

    for (long i = 11; i < 20; i += 2) {
        printList(l);
        assert(indexOf(l, (oop)i) == -1);
        printf("Inserting %ld\n", i);
        l = insertSorted(l, (oop)i);
    }

    printf("Inserting a bunch of 15's\n");
    l = insertSorted(l, (oop)15);
    l = insertSorted(l, (oop)15);
    l = insertSorted(l, (oop)15);
    l = insertSorted(l, (oop)15);
    l = insertSorted(l, (oop)15);
    l = insertSorted(l, (oop)15);
    l = insertSorted(l, (oop)15);

    for (long i = 0; i < 30; i += 1) {
        printList(l);
        printf("Inserting %ld\n", i);
        l = insertSorted(l, (oop)i);
    }

    for (int i = 0; i < l->used; i++) {
        assert(l->data[indexOf(l, l->data[i])] == l->data[i]);
    }

    assert(indexOf(l, (oop)42) == -1);
    assert(indexOf(l, (oop)420) == -1);

    printList(l);

    printf("\nTrying actual pointers\n\n");

    List *l2 = newList(10);
    for (int i = 0; i < 10; i++) {
        insertSorted(l2, newString("asdf"));
        printList(l2);
    }

    for (int i = 0; i < l2->used; i++) {
        assert(indexOf(l2, l2->data[i]) == i);
    }

    List *symbols = newList(10);

    oop s = newSymbol("a");

    push(symbols, s);

    assert(indexOfByValue(symbols, s) == 0);

    oop s2 = newSymbol("a");
    assert(indexOfByValue(symbols, s2) == 0);

    return 0;
}
