#include <stdio.h>
#include <stdlib.h>

#define DECLARE_VECTOR(NAME, TYPE)						\
										\
typedef struct NAME {								\
    int   size, capacity;							\
    TYPE *elements;								\
} NAME;										\
										\
NAME *NAME##_new() { return calloc(1, sizeof(struct NAME)); }			\
										\
TYPE NAME##_push(NAME *v, TYPE e)						\
{										\
    if (v->size >= v->capacity) {						\
	v->capacity = v->capacity ? v->capacity * 2 : 32;			\
	v->elements = realloc(v->elements, sizeof(*v->elements) * v->capacity);	\
    }										\
    return v->elements[v->size++] = e;						\
}										\
										\
TYPE NAME##_pop(NAME *v)							\
{										\
    if (!v->size) abort();							\
    return v->elements[--v->size];						\
}

DECLARE_VECTOR(IntVec, int)
DECLARE_VECTOR(StrVec, char *)

int main()
{
    IntVec *v = IntVec_new();

    for (int i = 0;  i < 10;  ++i) IntVec_push(v, i*i);

    while (v->size) printf("%d\n", IntVec_pop(v));

    StrVec *u = StrVec_new();

    StrVec_push(u, "foo");
    StrVec_push(u, "bar");
    StrVec_push(u, "baz");

    for (int i = 0;  i < u->size;  ++i) printf("%s\n", u->elements[i]);
    while (u->size) printf("%s\n", StrVec_pop(u));

    return 0;
}
