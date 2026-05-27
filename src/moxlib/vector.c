#include "moxlib/vector.h"
#include "moxlib/memory.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct vector {
    void *data;
    size_t len, capacity, size;
} vector;

vector *vector_create(size_t size) {
    vector *vec = AllocMem(sizeof(vector));
    
    vec->len = 0;
    vec->capacity = 2;
    vec->size = size;
    
    size_t data_size = vec->capacity * size;
    vec->data = AllocMem(data_size);
    memset(vec->data, 0, data_size);
    
    return vec;
}
static void vector_increase(vector *vector) {
    if (vector->len + 1 > vector->capacity) {
        ReallocMem(&vector->data, vector->capacity*2 * vector->size);
        vector->capacity *= 2;
    }
    vector->len ++;
}
void *vector_get(const vector *vector, size_t ind) {
    return (char*)vector->data + (ind * vector->size);
}
void *vector_at(const vector *vector, size_t ind) {
    assert(ind < vector->len);
    return (char*)vector->data + (ind * vector->size);
}
void vector_set(vector *vector, size_t ind, void *val) {
    assert(ind < vector->len);
    memcpy(vector_get(vector, ind), val, vector->size);
}
void vector_push(vector *vector, void *value) {
    vector_increase(vector);
    vector_set(vector, vector->len - 1, value);
}
void vector_insert(vector *vector, size_t ind, void *val) {
    assert(ind <= vector->len);
    
    vector_increase(vector);
    void *dst = vector_get(vector, ind + 1);
    void *src = vector_get(vector, ind);
    size_t bytes = (vector->len - ind - 1) * vector->size;
    memmove(dst, src, bytes);
    memcpy(src, val, vector->size);
}
void vector_remove(vector *vector, size_t ind) {
    if (ind < vector->len - 1) {
        void *dst = vector_get(vector, ind);
        void *src = vector_get(vector, ind + 1);
        memmove(dst, src, vector->size * (vector->len-ind-1));
    }
    vector->len --;
}
void *vector_pop(vector *vector) {
    assert(vector->len > 0);
    vector->len --;
    return &((char*)vector->data)[vector->len * vector->size];
}
void vector_fill(vector *vector, void *value) {
    for (size_t i = 0; i < vector->len; i ++) {
        memcpy(vector_get(vector, i), value, vector->size);
    }
}
void vector_swap(vector *vector, size_t i, size_t j) {
    void *p1 = vector_at(vector, i);
    void *p2 = vector_at(vector, j);
    
    for (size_t k = 0; k < vector->size; k ++) {
        char temp = ((char*)p1)[k];
        ((char*)p1)[k] = ((char*)p2)[k];
        ((char*)p2)[k] = temp;
    }
}
void vector_remove_unordered(vector *vector, size_t ind) {
    size_t last_index = vector->len - 1;
    vector_swap(vector, ind, last_index);
    vector_pop(vector);
}
void vector_reverse(vector *vector) {
    size_t half = vector->len / 2;
    for (size_t i = 0; i < half; i ++) {
        size_t opp = vector->len - i - 1;
        vector_swap(vector, i, opp);
    }
}
void vector_destroy(vector *vector) {
    FreeMem(&vector->data);
    vector->data = NULL;
    vector->len = 0;
    vector->capacity = 0;
    vector->size = 0;
    FreeMem(&vector);
}
void vector_sort(vector *vector, int (*cmp)(const void *a, const void *b)) {
    qsort(vector->data, vector->len, vector->size, cmp);
}
size_t vector_len(vector *vector) {
    return vector->len;
}