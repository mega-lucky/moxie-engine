#include "memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *AllocMem(size_t size) {
    void *memory = malloc(size);
    
    if (memory == NULL) {
        abort();
    }
    
    return memory;
}
void *CallocMem(size_t nblocks, size_t blocksize) {
    void *memory = calloc(nblocks, blocksize);
    
    if (memory == NULL) {
        abort();
    }
    
    return memory;
}
void FreeMem(void *memory) {
    free(*(void **)memory);
    *(void **)memory = NULL;
}
int ReallocMem(void *memory, size_t size) {
    void *newmemory = realloc(*(void **)memory, size);

    if (newmemory == NULL) {
        return 0;
    }

    *(void **)memory = newmemory;
    return 1;
}
void *DupMem(void *memory, size_t size) {
    void *ptr = AllocMem(size);
    memcpy(ptr, memory, size);
    return ptr;
}

MemoryArena *CreateMemoryArena(size_t size) {
    MemoryArena *arena = AllocMem(sizeof(MemoryArena));
    arena->data = CallocMem(1, size);
    arena->capacity = size;
    arena->size = 0;

    return arena;
}
void DestroyMemoryArena(MemoryArena *arena) {
    if (arena->data != NULL) { 
        memset(arena->data, 0, arena->capacity);
        FreeMem(&arena->data);
    }
    arena->capacity = 0;
    arena->size = 0;
    FreeMem(&arena);
}
void *PushToMemoryArena(MemoryArena *arena, size_t size) {
    if (size + arena->size > arena->capacity) {
        return NULL;
    }

    void *memory = arena->data + arena->size;
    arena->size += size;

    return memory;
}