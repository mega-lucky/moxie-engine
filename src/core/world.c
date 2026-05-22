#include "world.h"
#include "memory.h"

#include <string.h>
#include <stdlib.h>

typedef struct Component {
    size_t entities_count;
    EntityID *entities;
    char *data;
    ComponentDescription description;
} Component;
typedef struct System {
    SystemDescription desc;
    void *data;
} System;

#define AVAILABLE_IDS_BLOCKSIZE   sizeof(EntityID) * MAX_ENTITIES
#define ENTITY_IDS_BLOCKSIZE      sizeof(EntityID) * MAX_ENTITIES
#define COMPONENT_MASKS_BLOCKSIZE sizeof(ComponentMask) * MAX_ENTITIES
#define COMPONENTS_BLOCK_SIZE     sizeof(Component) * MAX_COMPONENTS
#define INDEX_BLOCK_SIZE          sizeof(size_t) * MAX_ENTITIES
#define SYSTEM_BLOCK_SIZE         sizeof(System) * MAX_SYSTEMS
#define QUERY_BLOCK_SIZE          sizeof(EntityQuery) * MAX_QUERIES

static bool is_singleton_mask(ComponentMask mask) {
    return mask != 0 && (mask & (mask - 1)) == 0;
}
static int systems_cmp(const void *a, const void *b) {
    const System *first = a;
    const System *second = b;

    int diff = first->desc.Phase - second->desc.Phase;
    return diff == 0 ? second->desc.Priority - first->desc.Priority : diff;
}
static size_t component_index_from_mask(ComponentMask mask) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(mask);
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward64(&index, mask);
    return (uint32_t)index;
#else
    static const uint32_t debruijn_table[64] = {
         0,  1, 56,  2, 57, 49, 28,  3, 61, 58, 42, 50, 38, 29, 17,  4,
        62, 47, 59, 36, 45, 43, 51, 22, 53, 39, 33, 30, 24, 18, 12,  5,
        63, 55, 48, 27, 60, 41, 37, 16, 46, 35, 44, 21, 52, 32, 23, 11,
        54, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19,  9, 13,  8,  7,  6
    };
    return debruijn_table[((uint64_t)((mask & -mask) * 0x03F79D71B4CA8B09ULL)) >> 58];
#endif
}
void add_to_query(EntityQuery *query, EntityID entity) {
    query->Index[entity] = EntityList_len(query->Entities);
    EntityList_push(query->Entities, entity);
}

void remove_from_query(EntityQuery *query, size_t entity) {
    const size_t query_index = query->Index[entity];
    if (EntityList_get(query->Entities, query_index) != entity) { return; }

    const EntityID back_entity = EntityList_at(query->Entities, EntityList_len(query->Entities)-1);
    query->Index[back_entity] = query_index;
    EntityList_remove_unordered(query->Entities, query_index);
}

static EntityID *available_ids = NULL;
static EntityID *entity_ids = NULL;
static ComponentMask *component_masks = NULL;
static Component *components = NULL;
static size_t *entity_indices = NULL;
static System *systems = NULL;
static EntityQuery *queries = NULL;

static size_t entity_count = 0;
static size_t component_count = 0;
static size_t system_count = 0;
static size_t query_count = 0;

static bool systems_sorted = true;

static MemoryArena *world_arena = NULL;

void InitWorld() {
    size_t arena_size = 
        AVAILABLE_IDS_BLOCKSIZE +
        ENTITY_IDS_BLOCKSIZE +
        COMPONENTS_BLOCK_SIZE +
        COMPONENT_MASKS_BLOCKSIZE +
        INDEX_BLOCK_SIZE +
        SYSTEM_BLOCK_SIZE +
        QUERY_BLOCK_SIZE;
    
    world_arena = CreateMemoryArena(arena_size);

    available_ids = PushToMemoryArena(world_arena, AVAILABLE_IDS_BLOCKSIZE);
    entity_ids = PushToMemoryArena(world_arena, ENTITY_IDS_BLOCKSIZE);
    components = PushToMemoryArena(world_arena, COMPONENTS_BLOCK_SIZE);
    component_masks = PushToMemoryArena(world_arena, COMPONENT_MASKS_BLOCKSIZE);
    entity_indices = PushToMemoryArena(world_arena, INDEX_BLOCK_SIZE);
    systems = PushToMemoryArena(world_arena, SYSTEM_BLOCK_SIZE);
    queries = PushToMemoryArena(world_arena, QUERY_BLOCK_SIZE);

    entity_count = 0;
    component_count = 0;
    for (size_t i = 0; i < MAX_ENTITIES; i ++)
        available_ids[i] = (MAX_ENTITIES - i - 1);
}
void TerminateWorld() {
    for (size_t i = 0; i < component_count; i ++) {
        Component *component = &components[i];

        if (component->description.Del) {
            for (size_t j = 0; j < component->entities_count; j ++) {
                EntityID entity = component->entities[j];
                component->description.Del(GetComponent(entity, 1 << i));
            }
        }

        if (component->entities != NULL) {
            FreeMem(&component->entities);
        }
        if (component->data != NULL) {
            FreeMem(&component->data);
        }
    }
    for (size_t i = 0; i < query_count; i ++) {
        EntityQuery *query = &queries[i];
        if (query->Entities.base != NULL) {
            EntityList_destroy(&query->Entities);
        }
        if (query->Index != NULL) {
            FreeMem(&query->Index);
        }
    }
    for (size_t i = 0; i < system_count; i ++) {
        System *system = &systems[i];

        if (system->data != NULL) {
            FreeMem(&system->data);
        }
        if (system->desc.Name != NULL) {
            FreeMem(&system->desc.Name);
        }
    }
    DestroyMemoryArena(world_arena);

    available_ids = NULL;
    entity_ids = NULL;
    component_masks = NULL;
    components = NULL;
    entity_indices = NULL;
    systems = NULL;
    queries = NULL;

    entity_count = 0;
    component_count = 0;
    system_count = 0;
    query_count = 0;
    systems_sorted = true;
}
EntityID NewEntity() {
    const size_t new_id_index = MAX_ENTITIES - 1 - entity_count;
    const EntityID id = available_ids[new_id_index];

    entity_indices[id] = entity_count;
    entity_ids[entity_count++] = id;

    return id;
}
void DestroyEntity(EntityID entity) {
    const EntityID back_entity = entity_ids[entity_count - 1];
    const size_t new_index = entity_indices[entity];
    const size_t old_index = entity_indices[back_entity];
    const ComponentMask mask = component_masks[new_index];
    const ComponentMask back_mask = component_masks[old_index];

    entity_indices[back_entity] = new_index;
    component_masks[new_index] = back_mask;

    for (size_t i = 0; i < component_count; i ++) {
        if (!(back_mask & (1 << i))) { continue; }

        const Component *current_component = &components[i];

        void *data = GetComponent(entity, 1 << i);
        ComponentFunc destructor = current_component->description.Del;
        if (destructor && data) {
            destructor(data);
        }

        const size_t data_size = current_component->description.DataSize;
        char *component_data = current_component->data;

        memmove(
            &component_data[new_index * data_size],
            &component_data[old_index * data_size],
            data_size
        );
    }

    for (size_t i = 0; i < query_count; i ++) {
        EntityQuery *query = &queries[i];
        if ((query->Mask & mask) != query->Mask) {
            continue;
        }

        remove_from_query(query, entity);
    }

    entity_count --;
}
EntityQuery *QueryEntities(ComponentMask mask) {
    for (int i = 0; i < query_count; i ++) {
        if (queries[i].Mask == mask) { return &queries[i]; }
    }

    Component *smallest_comp = NULL;
    size_t smallest_comp_count = SIZE_MAX;
    for (size_t i = 0; i < component_count; i ++) {
        if (!(mask & (1 << i))) { continue; }

        Component *current_component = &components[i];
        if (current_component->entities_count >= smallest_comp_count) {
            continue;
        }

        smallest_comp_count = current_component->entities_count;
        smallest_comp = current_component;
    }

    EntityQuery *query = &queries[query_count++];
    query->Entities = EntityList_new();
    query->Index = CallocMem(MAX_ENTITIES, sizeof(size_t));
    query->Mask = mask;

    if (!smallest_comp) {
        return query;
    }

    for (size_t i = 0; i < smallest_comp_count; i ++) {
        const EntityID entity = smallest_comp->entities[i];

        if (MatchesComponentMask(entity, mask)) {
            query->Index[entity] = EntityList_len(query->Entities);
            EntityList_push(query->Entities, entity);
        }
    }

    return query;
}
ComponentMask RegisterComponent(ComponentDescription description) {
    Component new_component;
    new_component.description = description;
    new_component.entities_count = 0;

    new_component.entities = CallocMem(MAX_ENTITIES, sizeof(EntityID));
    new_component.data = CallocMem(MAX_ENTITIES, description.DataSize);
    
    const size_t component_index = component_count;
    components[component_count++] = new_component;

    return (1 << component_index);
}
ComponentMask RegisterSingleton(ComponentDescription description) {
    Component new_component;
    new_component.description = description;
    new_component.entities_count = 0;
    new_component.entities = NULL;

    ComponentFunc init = description.Init;
    new_component.data = CallocMem(1, description.DataSize);
    if (init) {
        init(new_component.data);
    }
    
    const size_t component_index = component_count;
    components[component_count++] = new_component;

    return (1 << component_index);
}
void EntityGiveComponents(EntityID entity, ComponentMask mask) {
    if (MatchesComponentMask(entity, mask)) { return; }

    const size_t entity_index = entity_indices[entity];
    const ComponentMask old_mask = component_masks[entity_index];
    
    for (size_t i = 0; i < component_count; i ++) {
        if (!(mask & (1 << i))) { continue; }
        
        Component *component = &components[i];
        component->entities[component->entities_count++] = entity;

        ComponentFunc init = component->description.Init;

        if (!init) { continue; }
        size_t start_byte = entity_indices[entity] * component->description.DataSize;
        void *data = &component->data[start_byte];
        
        init(data);
    }
    
    component_masks[entity_index] |= mask;
    const ComponentMask new_mask = component_masks[entity_index];

    for (size_t i = 0; i < query_count; i ++) {
        EntityQuery *query = &queries[i];
        if (
            (query->Mask & new_mask) == query->Mask &&
            (query->Mask & old_mask) != query->Mask
        ) {
            add_to_query(query, entity);
        }
    }
}
bool MatchesComponentMask(EntityID entity, ComponentMask mask) {
    const size_t entity_index = entity_indices[entity];
    const ComponentMask entity_mask = component_masks[entity_index];

    return (mask & entity_mask) ==  mask;
}
void *GetComponent(EntityID entity, ComponentMask mask) {
    if (!is_singleton_mask(mask)) { return NULL; }
    if (!MatchesComponentMask(entity, mask)) { return NULL; }

    size_t component_index = component_index_from_mask(mask);
    size_t entity_index = entity_indices[entity];

    Component *component = &components[component_index];
    return &component->data[entity_index * component->description.DataSize];
}
void *GetSingleton(ComponentMask mask) {
    if (!is_singleton_mask(mask)) { return NULL; }
    size_t component_index = component_index_from_mask(mask);
    Component *component = &components[component_index];
    if (component->entities != NULL) { return NULL; }

    return component->data;
}
void RegisterSystem(SystemDescription desc) {
    void *data = desc.DataSize == 0 ? NULL : CallocMem(1, desc.DataSize);
    if (desc.Init != NULL) {
        desc.Init(data);
    }
    if (desc.Name != NULL) {
        desc.Name = DupMem(desc.Name, strlen(desc.Name + 1));
    }
    systems[system_count ++] = (System){ .desc=desc, .data=data };

    systems_sorted = false;
}
void UpdateSystems() {
    if (!systems_sorted) {
        qsort(systems, system_count, sizeof(System), systems_cmp);
        systems_sorted = true;
    }

    for (size_t i = 0; i < system_count; i ++) {
        if (systems[i].desc.Update == NULL) { continue; }
        systems[i].desc.Update(systems[i].data);
    }
}