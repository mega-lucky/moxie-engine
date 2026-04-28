#include "map.h"
#include "memory.h"
#include <string.h>
#include <assert.h>

uint64_t DEFAULT_HASH(const void *key, size_t size) {
    const uint8_t *bytes = key;
    uint64_t hash = 1469598103934665603ULL;
    for (size_t i = 0; i < size; i ++) {
        hash ^= bytes[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}
uint64_t CSTR_HASH(const void *key, size_t size) {
    const char *chars = *(const char* const*)key;
    size_t len = strlen(chars);
    uint64_t hash = 146958103934665603ULL;
    for (size_t i = 0; i < len; i ++) {
        hash ^= chars[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}
int DEFAULT_CMP(const void *a, const void *b, size_t size) {
    const uint8_t *bytes_a = (const uint8_t *)a;
    const uint8_t *bytes_b = (const uint8_t *)b;
    size_t i = 0;
    while (i < size && *bytes_a == *bytes_b) {
        bytes_a ++;
        bytes_b ++;
        i ++;
    }
    return *bytes_a - *bytes_b;
}
int CSTR_CMP(const void *a, const void *b, size_t size) {
    const char *str_a = *(const char* const*)a;
    const char *str_b = *(const char* const*)b;
    
    return strcmp(str_a, str_b);
}
void *DEFAULT_KCPY(const void *key, size_t size) {
    void *new_key = AllocMem(size);
    memcpy(new_key, key, size);
    return new_key;
}
void *CSTR_KCPY(const void *key, size_t size) {
    char **new_key = AllocMem(sizeof(const char *));
    *new_key = strdup(*(const char * const *)key);
    return new_key;
}
void DEFAULT_KFREE(void *key) {
    FreeMem(&key);
}
void CSTR_KFREE(void *key) {
    FreeMem(key);
    FreeMem(&key);
}

hash_map *new_hash_map(size_t keysize, size_t valuesize, size_t capacity) {
    hash_map *new_map = AllocMem(sizeof(hash_map));
    new_map->keysize = keysize;
    new_map->valuesize = valuesize;
    new_map->capacity = capacity;
    new_map->count = 0;
    new_map->data = AllocMem(capacity * sizeof(map_data));

    return new_map;
}
void destroy_hash_map(hash_map *map) {
    FreeMem(&map->data);
    map->capacity = 0;
    map->cmp_fn = NULL;
    map->hash_fn = NULL;
    map->keysize = 0;
    map->valuesize = 0;
    map->count = 0;
    FreeMem(&map);
}
void hash_map_set(hash_map *map, const void *key, const void *value) {
    assert(key != NULL && "Unable to use NULL as a key\n");

    HashFn hash_func = map->hash_fn
        ? map->hash_fn
        : DEFAULT_HASH;
    size_t hash = hash_func(key, map->keysize);
    size_t index = hash % map->capacity;

    CopyKeyFn kcpy_fn = map->kcpy_fn
        ? map->kcpy_fn
        : DEFAULT_KCPY;

    size_t probe_len = 0;
    while (map->data[index].key != NULL && probe_len < map->capacity) {
        // Collision occured
        index = (index + 1) % map->capacity;
        probe_len ++;
    }

    if (probe_len < map->capacity) {
        map->data[index].key = kcpy_fn(key, map->keysize);
        map->data[index].value = AllocMem(map->valuesize);
        memcpy(map->data[index].value, value, map->valuesize);
        map->count++;
    }
}
void hash_map_remove(hash_map *map, const void *key) {
    HashFn hash_func = map->hash_fn
        ? map->hash_fn
        : DEFAULT_HASH;

    size_t hash = hash_func(key, map->keysize);
    size_t index = hash % map->capacity;
    map_data *data = &map->data[index];

    FreeKeyFn kfree_fn = map->kfree_fn
        ? map->kfree_fn
        : DEFAULT_KFREE;

    if (data->key == NULL) { return; }
    kfree_fn(data->key);
    FreeMem(&data->value);
    data->key = NULL;
    data->value = NULL;
    map->count--;
}
void *hash_map_get(hash_map *map, const void *key) {
    size_t keysize = map->keysize;
    size_t capacity = map->capacity;

    HashFn hash_fn = map->hash_fn
        ? map->hash_fn
        : DEFAULT_HASH;

    size_t hash = hash_fn(key, map->keysize);
    size_t index = hash % map->capacity;

    CompareFn cmp_fn = map->cmp_fn
        ? map->cmp_fn
        : DEFAULT_CMP;

    size_t probe_len = 0;

    void *cur_key = map->data[index].key;
    size_t cur_hash = cur_key ? hash_fn(cur_key, keysize) : 0;
    int found = cur_key ? cmp_fn(key, cur_key, map->keysize) == 0 : 0;

    while (
        !found &&
        probe_len < map->capacity &&
        (cur_key == NULL || (cur_hash % capacity) == (hash % capacity))
    ) {
        index = (index + 1) % map->capacity;
        probe_len ++;
        cur_key = map->data[index].key;
        cur_hash = cur_key ? hash_fn(cur_key, map->keysize) : 0;
        found = cur_key ? cmp_fn(key, cur_key, map->keysize) == 0 : 0;
    }
    
    return found ? map->data[index].value : NULL;
}