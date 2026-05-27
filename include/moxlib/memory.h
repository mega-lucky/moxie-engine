#ifndef __MEMORY_H
#define __MEMORY_H

#include "stddef.h"

void *AllocMem(size_t size);
void *CallocMem(size_t nblocks, size_t blocksize);
void FreeMem(void *memory);
int ReallocMem(void *memory, size_t size);
void *DupMem(const void *memory, size_t size);

typedef struct MemoryArena {
    char *data;
    size_t size;
    size_t capacity;
} MemoryArena;

MemoryArena *CreateMemoryArena(size_t size);
void DestroyMemoryArena(MemoryArena *arena);
void *PushToMemoryArena(MemoryArena *arena, size_t size);

#endif