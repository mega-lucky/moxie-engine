#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include <memory>
#include <format>
#include "./ecs/types.h"
#include "./ecs/component.h"

template<typename T>
void World::RegisterComponent() {
    ComponentID id = GetComponentID<T>();

    if (id >= m_components.size()) {
        m_components.resize(id + 1);
    }

    m_components[id] = std::make_unique<ComponentPool<T>>(max_entities);
}

template<typename T>
bool World::EntityHasComponent(Entity entity) {
    ComponentID comp_id = GetComponentID<T>();
    Signature sig = m_signatures[entity];
    return sig.test(comp_id);
}

template<typename T>
T& World::GetComponent(Entity entity) {
    ComponentID id = GetComponentID<T>();
    auto* pool = static_cast<ComponentPool<T>*>(m_components[id].get());
    return pool->Get(entity);
}

template<typename T>
void World::GiveComponent(Entity entity) {
    if (EntityHasComponent<T>(entity)) { return; }

    ComponentID id = GetComponentID<T>();
    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(id, true);
    
    m_components.at(id)->Add(entity);
}

template<typename T>
void World::RemoveComponent(Entity entity) {
    if (!EntityHasComponent<T>(entity)) { return; }

    ComponentID id = GetComponentID<T>();
    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(id, false);
    
    m_components.at(id)->Remove(entity);
}

template<typename T>
ComponentID World::GetComponentID() {
    static ComponentID id = next_component_id();
    return id;
}

template<typename T>
void World::RegisterSystem() {
    system_desc new_desc = {
        .name = std::format("System {}", m_systems.size() + 1),
        .priority = 0,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

template<typename T>
void World::RegisterSystem(int priority) {
    system_desc new_desc = {
        .name = std::format("System {}", m_systems.size() + 1),
        .priority = priority,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

template<typename T>
void World::RegisterSystem(std::string name) {
    system_desc new_desc = {
        .name = name,
        .priority = 0,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

template<typename T>
void World::RegisterSystem(std::string name, int priority) {
    system_desc new_desc = {
        .name = name,
        .priority = priority,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

#endif