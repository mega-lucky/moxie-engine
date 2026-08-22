#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include "./types.h"
#include <stdexcept>

namespace World {

template<typename T>
Registry::ComponentPool<T>::ComponentPool(size_t max_entities) {
    m_data.reserve(max_entities);
    m_entities.reserve(max_entities);
    m_sparse.resize(max_entities, -1);
}
template<typename T>
void Registry::ComponentPool<T>::Add(Entity entity, const T& value) {
    if (Has(entity)) { return; }

    m_sparse[entity] = static_cast<int32_t>(m_data.size());
    m_data.push_back(value);
    m_entities.push_back(entity);
}
template<typename T>
void Registry::ComponentPool<T>::Add(Entity entity, T&& value) {
    if (Has(entity)) { return; }

    m_sparse[entity] = static_cast<int32_t>(m_data.size());
    m_data.push_back(std::move(value));
    m_entities.push_back(entity);
}
template<typename T>
void Registry::ComponentPool<T>::Remove(Entity entity) {
    if (!Has(entity)) { return; }

    Entity last_entity = m_entities.back();

    int32_t index = m_sparse[entity];
    int32_t last_index = m_sparse[last_entity];
    
    m_data[index] = std::move(m_data[last_index]);
    m_entities[index] = last_entity;
    m_sparse[last_entity] = index;
    m_sparse[entity] = -1;

    m_data.pop_back();
    m_entities.pop_back();
}
template<typename T>
bool Registry::ComponentPool<T>::Has(Entity entity) const {
    return m_sparse[entity] != -1;
}

template<typename T>
size_t Registry::ComponentPool<T>::Size() const {
    return m_entities.size();
}

template<typename T>
const std::vector<Entity>& Registry::ComponentPool<T>::Entities() const {
    return m_entities;
}

template<typename T>
const T& Registry::ComponentPool<T>::Get(Entity entity) const {
    if (!Has(entity)) {
        throw std::runtime_error("Entity has no such component");
    }

    int32_t index = m_sparse[entity];
    return m_data[index];
}

template<typename T>
T& Registry::ComponentPool<T>::Get(Entity entity) {
    return const_cast<T&>(std::as_const(*this).Get(entity));
}

}

#endif