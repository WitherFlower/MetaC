#ifndef VEC_H
#define VEC_H

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

#endif

