#ifndef __ECS_COMPONENT__
#define __ECS_COMPONENT__

#include "./types.h"

namespace World {

template<typename T>
const T& ComponentPool::Get(Entity entity) const {
    const void *ptr = Get(entity);
    return *static_cast<const T*>(ptr);
}
template<typename T>
T& ComponentPool::Get(Entity entity) {
    void *ptr = Get(entity);
    return *static_cast<T*>(ptr);
}

template<typename T>
void ComponentPool::Add(Entity entity, const T& data) {
    const void *ptr = static_cast<const void*>(&data);
    Add(entity, ptr);
}

template<typename T>
void ComponentPool::Add(Entity entity, T&& data) {
    if (Has(entity)) {
        throw std::runtime_error("Entity already has this component");
    }
    
    if (m_sparse.size() <= entity) {
        m_sparse.resize(entity + 1, -1);
    }
    m_sparse[entity] = static_cast<int32_t>(m_entities.size());
    size_t offset = m_entities.size() * m_stride;

    m_entities.push_back(entity);
    m_data.resize(m_data.size() + m_stride);

    move(m_data.data() + offset, &data);
}

}

#endif