#ifndef __WORLD_H
#define __WORLD_H

#define MAX_ENTITIES 200
#define MAX_COMPONENTS 64
#define MAX_SYSTEMS 70
#define MAX_QUERIES 20

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"

enum CyclePhase {
    CYCLE_PHASE_FIRST,
    CYCLE_PHASE_INPUT,
    CYCLE_PHASE_PHYSICS,
    CYCLE_PHASE_PRERENDER,
    CYCLE_PHASE_RENDER,
    CYCLE_PHASE_LAST,
};

typedef uint32_t EntityID;
typedef uint64_t ComponentMask;
DEFINE_VECTOR(EntityID, EntityList);

typedef void (*SystemUpdate)(void *data);
typedef void (*ComponentDestructor)(void *data);

typedef struct SystemDescription {
    enum CyclePhase Phase;
    size_t DataSize;
    int Priority;
    SystemUpdate Init;
    SystemUpdate Update;
} SystemDescription;

typedef struct ComponentDescription {
    size_t DataSize;
    ComponentDestructor Destructor;
    void *DefaultValue;
} ComponentDescription;

struct EntityQuery {
    ComponentMask Mask;
    size_t *Index;
    EntityList Entities;
};
typedef struct EntityQuery EntityQuery;

void InitWorld();
void TerminateWorld();
EntityID NewEntity();
void DestroyEntity(EntityID entity);
EntityQuery *QueryEntities(ComponentMask mask);
ComponentMask RegisterComponent(ComponentDescription description);
ComponentMask RegisterSingleton(ComponentDescription description);
void EntityGiveComponents(EntityID entity, ComponentMask mask);
bool MatchesComponentMask(EntityID entity, ComponentMask mask);
void *GetComponent(EntityID entity, ComponentMask mask);
void *GetSingleton(ComponentMask mask);
void RegisterSystem(SystemDescription description);
void UpdateSystems();

#endif