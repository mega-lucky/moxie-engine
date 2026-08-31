#include "./world.hpp"
#include <stdexcept>

World::Registry::Registry() {
    m_components.reserve(max_components);
    m_entities.reserve(max_entities);
    m_signatures.resize(max_entities, 0);
    m_systems.reserve(max_systems);
    m_index_array.resize(max_entities, -1);
    m_entity_queue.reserve(max_entities);
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
        pool.Remove(entity);
    }
}

void World::Registry::GiveComponent(Entity entity, ComponentID comp_id, const void *value) {
    if (EntityHasComponent(entity, comp_id)) {
        throw std::runtime_error("Entity already has this component");
    }

    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(comp_id, true);

    m_components[comp_id].Add(entity, value);
}

const void *World::Registry::GetComponent(Entity entity, ComponentID comp_id) const {
    if (!IsComponent(comp_id)) {
        throw std::runtime_error("Invalid Component ID");
    }

    return m_components[comp_id].Get(entity);
}

void *World::Registry::GetComponent(Entity entity, ComponentID comp_id) {
    if (!IsComponent(comp_id)) {
        throw std::runtime_error("Invalid Component ID");
    }

    return m_components[comp_id].Get(entity);
}

bool World::Registry::IsComponent(ComponentID id) const noexcept {
    return id < m_components.size();
}

void World::Registry::RemoveComponent(Entity entity, ComponentID comp_id) {
    if (!EntityHasComponent(entity, comp_id)) { return; }

    Signature &entity_sig = m_signatures[m_index_array[entity]];
    entity_sig.set(comp_id, false);
    
    m_components[comp_id].Remove(entity);
}

bool World::Registry::EntityHasComponent(Entity entity, ComponentID comp_id) const noexcept {
    if (!IsComponent(comp_id)) {
        return false;
    }
    Signature sig = m_signatures[m_index_array[entity]];
    return sig.test(comp_id);
}

bool World::Registry::EntityMatchesSignature(Entity entity, Signature signature) const noexcept {
    Signature entity_sig = m_signatures[m_index_array[entity]];
    return (entity_sig & signature) == signature;
}

ComponentID World::Registry::RegisterComponent(const ComponentDescription &desc) {
    auto id = static_cast<ComponentID>(m_components.size());
    m_components.emplace_back(desc);
    return id;
}

ComponentID World::Registry::RegisterComponent(ComponentDescription &&desc) {
    auto id = static_cast<ComponentID>(m_components.size());
    m_components.emplace_back(std::move(desc));
    return id;
}

void World::Registry::StoreComponentID(ComponentID id, std::string name) {
    if (!IsComponent(id)) {
        throw std::runtime_error("Given ID is not a valid component id");
    }
    m_component_registry[name] = id;
}

ComponentID World::Registry::GetComponentID(std::string name) {
    auto it = m_component_registry.find(name);

    if (it == m_component_registry.end()) {
        throw std::runtime_error("No component id was stored with this name");
    };
    
    return it->second;
}

World::ComponentPool::ComponentPool(const World::ComponentDescription &desc) : m_description(desc) {
    m_data.reserve(desc.block_size * 8);
    m_entities.reserve(8);
    m_sparse.resize(8, -1);
}

World::ComponentPool::ComponentPool(World::ComponentDescription &&desc) : m_description(std::move(desc)) {
    m_data.reserve(desc.block_size * 8);
    m_entities.reserve(8);
    m_sparse.resize(8, -1);
}

bool World::ComponentPool::Has(Entity entity) const noexcept {
    if (m_sparse.size() <= entity) {
        return false;
    }
    return m_sparse[entity] != -1;
}

const void *World::ComponentPool::Get(Entity entity) const {
    if (!Has(entity)) {
        throw std::runtime_error("Entity does not have this specified component");
    }

    int32_t index = m_sparse[entity];
    size_t offset = m_description.block_size * index;

    return static_cast<const void *>(m_data.data() + offset);
}
void *World::ComponentPool::Get(Entity entity) {
    return const_cast<void *>(std::as_const(*this).Get(entity));
}

void World::ComponentPool::Add(Entity entity, const void *data) {
    if (Has(entity)) {
        throw std::runtime_error("Entity already has this component");
    }
    
    if (m_sparse.size() <= entity) {
        m_sparse.resize(entity + 1, -1);
    }
    m_sparse[entity] = static_cast<int32_t>(m_entities.size());
    size_t offset = m_entities.size() * m_description.block_size;

    m_entities.push_back(entity);
    m_data.resize(m_data.size() + m_description.block_size);

    if (data == nullptr) {
        m_description.constructor(m_data.data() + offset);
    } else {
        m_description.copy(m_data.data() + offset, data); 
    }
}

void World::ComponentPool::Remove(Entity entity) {
    if (!Has(entity)) {
        throw std::runtime_error("Entity doesnt have this component");
    }

    int32_t index = m_sparse.at(entity);
    Entity back_entity = m_entities.back();
    int32_t back_index = m_sparse.at(back_entity);
    size_t block_size = m_description.block_size;

    m_description.destructor(m_data.data() + index * block_size);

    if (index != back_index) {
        std::swap(m_entities[index], m_entities.back());

        m_sparse[back_entity] = index;

        auto target_range_start = m_data.begin() + (index * block_size);
        auto target_range_end   = target_range_start + block_size;
        auto last_range_start   = m_data.begin() + (back_index * block_size);

        std::swap_ranges(target_range_start, target_range_end, last_range_start);
    }

    m_entities.pop_back();
    m_sparse[entity] = -1;
}

const std::vector<Entity> &World::ComponentPool::Entities() const noexcept {
    return m_entities;
}

World::Query::Query(const World::Registry &reg, const Signature &comp_mask) :
    m_pool(nullptr),
    m_world(&reg),
    m_signature(comp_mask)
{
    for (ComponentID id = 0; id < reg.m_components.size(); id ++) {
        if (!comp_mask.test(id)) { continue; }

        if (!reg.IsComponent(id)) {
            throw std::runtime_error("Invalid Component ID");
        }

        const ComponentPool &pool = reg.m_components[id];
        
        if (m_pool == nullptr || pool.Entities().size() < m_pool->Entities().size()) {
            m_pool = &pool;
        }
    }
}

World::Query::Query(const World::Registry &reg, std::initializer_list<ComponentID> comp_ids) :
    m_pool(nullptr),
    m_world(&reg)
{
    for (auto id : comp_ids) {
        if (!reg.IsComponent(id)) {
            throw std::runtime_error("Invalid Component ID");
        }
        const ComponentPool &pool = reg.m_components[id];
        
        if (m_pool == nullptr || pool.Entities().size() < m_pool->Entities().size()) {
            m_pool = &pool;
        }

        m_signature.set(id, true);
    }
}

void World::Query::Each(const std::function<void(Entity)> &callback) {
    if (!m_pool) {
        return;
    }

    for (auto entity : m_pool->Entities()) {
        if (!m_world->EntityMatchesSignature(entity, m_signature)) {
            continue;
        }
        if (callback) { callback(entity); }
    }
}
