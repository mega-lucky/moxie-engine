#ifndef __VECTOR_H
#define __VECTOR_H

#include <stddef.h>

typedef struct vector vector;

vector *vector_create(size_t size);
void *vector_get(const vector *vector, size_t index);
void *vector_at(const vector *vector, size_t index);
void vector_set(vector *vector, size_t index, void *val);
void vector_push(vector *vector, void *value);
void vector_insert(vector *vector, size_t index, void *val);
void vector_fill(vector *vector, void *value);
void vector_remove(vector *vector, size_t index);
void vector_remove_unordered(vector *vector, size_t index);
void *vector_pop(vector *vector);
void vector_sort(vector *vector, int(*comp)(const void *a, const void *b));
void vector_destroy(vector *vector);
void vector_swap(vector *vector, size_t i, size_t j);
void vector_reverse(vector *vector);
size_t vector_len(vector *vector);

#define DEFINE_VECTOR(T, name) \
typedef struct name { \
    vector *base; \
} name; \
static inline name name##_new() { \
    vector *base = vector_create(sizeof(T)); \
    return (name){ .base = base }; \
} \
static inline void name##_push(name vec, T val) { \
    vector_push(vec.base, &val); \
} \
static inline void name##_insert(name vec, size_t index, T val) { \
    vector_insert(vec.base, index, &val); \
} \
static inline void name##_set(name vec, size_t index, T val) { \
    vector_set(vec.base, index, &val); \
} \
static inline void name##_fill(name vec, T value) { \
    vector_fill(vec.base, &value); \
} \
static inline void name##_remove(name vec, size_t index) { \
    vector_remove(vec.base, index); \
} \
static inline T name##_pop(name vec) { \
    return *(T*)vector_pop(vec.base); \
} \
static inline void name##_remove_unordered(name vec, size_t index) { \
    vector_remove_unordered(vec.base, index); \
} \
static inline void name##_swap(name vec, size_t i, size_t j) { \
    vector_swap(vec.base, i, j); \
} \
static inline void name##_reverse(name vec) { \
    vector_reverse(vec.base); \
} \
static inline T name##_get(name vec, size_t index) { \
    return *(T*)vector_get(vec.base, index); \
} \
static inline T name##_at(name vec, size_t index) { \
    return *(T*)vector_at(vec.base, index); \
} \
static inline void name##_destroy(name *vec) { \
    vector_destroy(vec->base); \
    vec->base = NULL; \
} \
static inline void name##_sort(name vec, int (*cmp)(const void *a, const void *b)) { \
    vector_sort(vec.base, cmp); \
} \
static inline size_t name##_len(name vec) { \
    return vector_len(vec.base); \
}

#endif