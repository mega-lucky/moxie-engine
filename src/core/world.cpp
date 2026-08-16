#include "./world.hpp"

World::Registry::Registry() {
    m_components.reserve(max_components);
    m_entities.reserve(max_entities);
    m_signatures.resize(max_entities, 0);
    m_systems.reserve(max_systems);
    m_index_array.resize(max_entities, -1);
    m_entity_queue.reserve(max_entities);
}

ComponentID World::Registry::next_component_id() {
    static ComponentID next = 0;
    return next ++;
}

Entity World::Registry::NewEntity() {
    if (m_entities.size() + 1 > max_entities) {
        throw std::runtime_error("Exceeded maximum entity count");
    }

    Entity new_entity;
    
    if (m_entity_queue.empty()) {
        new_entity = static_cast<Entity>(m_entities.size());
    } else {
        new_entity = m_entity_queue.back();
        m_entity_queue.pop_back();
    }

    m_index_array[new_entity] = static_cast<int32_t>(m_entities.size());
    m_entities.push_back(new_entity);

    return new_entity;
}

void World::Registry::Update(double dt) {
    for (auto &sys_desc : m_systems) {
        sys_desc.system->Update(dt);
    }
}

void World::Registry::DeleteEntity(Entity entity) {
    if (m_index_array[entity] == -1) {
        throw std::runtime_error("Attempt to delete non-existent entity");
    }

    Entity last_entity = m_entities.back();
    int32_t new_index = m_index_array[entity];
    int32_t old_index = m_index_array[last_entity];
    m_index_array[last_entity] = new_index;

    m_entities[new_index] = last_entity;
    m_entities.pop_back();

    m_signatures[new_index] = m_signatures[old_index];
    
    m_index_array[entity] = -1;
    m_entity_queue.push_back(entity);

    for (auto& pool : m_components) {
        pool->Remove(entity);
    }
}