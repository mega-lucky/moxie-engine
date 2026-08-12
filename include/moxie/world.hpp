#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include <memory>
#include <format>
#include "./ecs/types.h"
#include "./ecs/component.h"

namespace World {

template<typename T>
void Registry::RegisterComponent() {
    ComponentID id = get_component_id<T>();
    m_component_registry.set(static_cast<size_t>(id), true);

    if (id >= m_components.size()) {
        m_components.resize(id + 1);
    }

    m_components[id] = std::make_unique<ComponentPool<T>>(max_entities);
}

template<typename T>
bool Registry::EntityHasComponent(Entity entity) {
    ComponentID comp_id = get_component_id<T>();
    Signature sig = m_signatures[entity];
    return sig.test(comp_id);
}

template<typename T>
T& Registry::GetComponent(Entity entity) {
    if (!IsComponent<T>()) {
        throw std::runtime_error("GetComponent called on a type that isnt a component");
    }

    ComponentID id = get_component_id<T>();

    auto* pool = static_cast<ComponentPool<T>*>(m_components[id].get());
    return pool->Get(entity);
}

template<typename T>
bool Registry::IsComponent() {
    ComponentID id = get_component_id<T>();

    return m_component_registry.test(id);
}

template<typename T>
void Registry::GiveComponent(Entity entity) {
    if (EntityHasComponent<T>(entity)) { return; }

    ComponentID id = get_component_id<T>();
    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(id, true);
    
    m_components.at(id)->Add(entity);
}

template<typename T>
void Registry::RemoveComponent(Entity entity) {
    if (!EntityHasComponent<T>(entity)) { return; }

    ComponentID id = get_component_id<T>();
    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(id, false);
    
    m_components.at(id)->Remove(entity);
}

template<typename T>
ComponentID Registry::get_component_id() {
    static ComponentID id = next_component_id();
    return id;
}

template<typename T>
void Registry::RegisterSystem() {
    system_desc new_desc = {
        .name = std::format("System {}", m_systems.size() + 1),
        .priority = 0,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

template<typename T>
void Registry::RegisterSystem(int priority) {
    system_desc new_desc = {
        .name = std::format("System {}", m_systems.size() + 1),
        .priority = priority,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

template<typename T>
void Registry::RegisterSystem(std::string name) {
    system_desc new_desc = {
        .name = name,
        .priority = 0,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}

template<typename T>
void Registry::RegisterSystem(std::string name, int priority) {
    system_desc new_desc = {
        .name = name,
        .priority = priority,
        .system = std::make_unique<T>()
    };
    m_systems.push_back(std::move(new_desc));
}


template<typename T>
World::Registry::ComponentPool<T>* World::Registry::get_pool() {
    if (!IsComponent<T>()) {
        throw std::runtime_error("Specified type is not a registered component");
    }

    ComponentID id = get_component_id<T>();
    return static_cast<ComponentPool<T>*>(m_components[id].get());
}

template<typename ...Args>
std::vector<Entity> World::Registry::NewQuery() {
    static_assert(sizeof...(Args) > 0, "Atleast 1 argument expected");

    std::array<IComponentPool*, sizeof...(Args)> relevant = {
        static_cast<IComponentPool*>(get_pool<Args>())...
    };

    auto *smallest = *std::min_element(
        relevant.begin(), relevant.end(),
        [](IComponentPool* a, IComponentPool* b) {
            return a->Size() < b->Size();
        });

    std::vector<Entity> result;
    result.reserve(smallest->Size());

    for (auto entity : smallest->Entities()) {
        bool matches = true;
        for (auto *pool : relevant) {
            if (pool != smallest && !pool->Has(entity)) {
                matches = false;
                break;
            }
        }
        if (matches) {
            result.push_back(entity);
        }
    }

    return result;
}

}

#endif