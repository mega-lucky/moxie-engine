#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include <utility>
#include "./ecs/types.h"
#include "./ecs/component.h"
#include <stdexcept>
#include <format>

namespace World {

template<typename T>
ComponentID Registry::RegisterComponent() {
    auto id = static_cast<ComponentID>(m_components.size());
    m_components.emplace_back(ComponentDescription{
        .data_size = sizeof(T),
        .alignment = alignof(T),
        .constructor = [](void *dest){ new (dest) T; },
        .destructor = [](void *ptr){
            auto *item = reinterpret_cast<T*>(ptr);
            item->~T();
        },
        .copy = [](void *dest, const void *original){
            new (dest) T(*reinterpret_cast<const T*>(original));
        },
        .move = [](void *dest, void *source){
            new (dest) T(std::move(*reinterpret_cast<T*>(source)));
        },
    });
    return id;
}

template<typename T>
const T& Registry::GetComponent(Entity entity, ComponentID comp_id) const {
    if (!IsComponent(comp_id)) {
        throw std::runtime_error("Invalid Component ID");
    }

    return m_components[comp_id].Get<T>(entity);
}

template<typename T>
T& Registry::GetComponent(Entity entity, ComponentID comp_id) {
    return const_cast<T&>(std::as_const(*this).GetComponent<T>(entity, comp_id));
}

template<typename T>
void Registry::GiveComponent(Entity entity, ComponentID comp_id, const T& value) {
    if (EntityHasComponent(entity, comp_id)) {
        GetComponent<T>(entity, comp_id) = value;
        return;
    }

    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(comp_id, true);

    m_components[comp_id].Add(entity, value);
}
template<typename T>
void Registry::GiveComponent(Entity entity, ComponentID comp_id, T&& value) {
    if (EntityHasComponent(entity, comp_id)) {
        GetComponent<T>(entity, comp_id) = std::move(value);
        return;
    }

    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(comp_id, true);

    m_components[comp_id].Add(entity, std::move(value));
}

template<typename T, typename... arg_types>
void Registry::RegisterSystem(arg_types&&... args) {
    system_desc new_desc = {
        .name = std::format("System {}", m_systems.size() + 1),
        .priority = 0,
        .system = std::make_unique<T>(std::forward<arg_types>(args)...)
    };
    m_systems.push_back(std::move(new_desc));
}

}

#endif
