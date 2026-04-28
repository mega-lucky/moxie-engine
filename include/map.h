#ifndef __MAP_H__
#define __MAP_H__

#include <stddef.h>
#include <stdint.h>

typedef size_t (*HashFn)(const void*, size_t);
typedef int (*CompareFn)(const void*, const void*, size_t);
typedef void *(*CopyKeyFn)(const void*, size_t);
typedef void (*FreeKeyFn)(void *);

typedef struct map_data {
    void *key;
    void *value;
} map_data;

typedef struct hash_map {
    HashFn hash_fn;
    CompareFn cmp_fn;
    CopyKeyFn kcpy_fn;
    FreeKeyFn kfree_fn;

    size_t keysize, valuesize, capacity, count;

    map_data *data;
} hash_map;

hash_map *new_hash_map(size_t keysize, size_t valsize, size_t capacity);
void destroy_hash_map(hash_map *map);
void hash_map_set(hash_map *map, const void *key, const void *value);
void hash_map_remove(hash_map *map, const void *key);
void *hash_map_get(hash_map *map, const void *key);

uint64_t DEFAULT_HASH(const void *key, size_t size);
uint64_t CSTR_HASH(const void *key, size_t size);

int DEFAULT_CMP(const void *a, const void *b, size_t size);
int CSTR_CMP(const void *a, const void *b, size_t size);

void *DEFAULT_KCPY(const void *key, size_t size);
void *CSTR_KCPY(const void *key, size_t size);

void DEFAULT_KFREE(void *key);
void CSTR_KFREE(void *key);

#define DEFINE_MAP_EX(K, V, name, HASH, CMP, KCPY, KFREE) \
typedef struct name { \
    hash_map *base; \
} name; \
static inline name name##_new(size_t capacity) { \
    hash_map *base = new_hash_map(sizeof(K), sizeof(V), capacity); \
    base->hash_fn = HASH; \
    base->cmp_fn = CMP; \
    base->kcpy_fn = KCPY; \
    base->kfree_fn = KFREE; \
    return (name){base}; \
} \
static inline void name##_destroy(name map) { \
    destroy_hash_map(map.base); \
    map.base = NULL;\
} \
static inline void name##_set(name map, K key, V value) { \
    hash_map_set(map.base, &key, &value); \
} \
static inline V name##_remove(name map, K key) { \
    V temp = *(V*)hash_map_get(map.base, &key); \
    hash_map_remove(map.base, &key); \
    return temp; \
} \
static inline V name##_get(name map, K key) { \
    V* ptr = hash_map_get(map.base, &key); \
    return ptr ? *(V*)ptr : (V){0}; \
}

#define DEFINE_MAP(K, V, name) DEFINE_MAP_EX(K, V, name, DEFAULT_HASH, DEFAULT_CMP, DEFAULT_KCPY, DEFAULT_KFREE)
#define DEFINE_CSTR_MAP(V, name) DEFINE_MAP_EX(const char*, V, name, CSTR_HASH, CSTR_CMP, CSTR_KCPY, CSTR_KFREE)

#endif