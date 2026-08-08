#include "moxie/world.hpp"

World::World() {
    m_components.reserve(max_components);
    m_entities.reserve(max_entities),
    m_signatures.resize(max_entities);
}

ComponentID World::next_component_id() {
    static ComponentID next = 0;
    return next ++;
}

Entity World::NewEntity() {
    Entity new_entity = static_cast<Entity>(m_entities.size());
    m_entities.push_back(new_entity);
    return new_entity;
}

void World::Update() {
    for (auto &sys : m_systems) {
        sys->Update();
    }
}

void World::DeleteEntity(Entity entity) {
    Signature &entity_sig = m_signatures[entity];
    entity_sig = 0;

    for (auto& pool : m_components) {
        pool->Remove(entity);
    }
}