#ifndef __ECS_TYPES__
#define __ECS_TYPES__

#include <cstdint>
#include <bitset>
#include <vector>
#include <memory>
#include <functional>

using Entity = uint32_t;
using ComponentID = uint32_t;
using Signature = std::bitset<128>;

struct lua_State;

namespace World {

    struct ComponentDescription {
    size_t block_size;
    std::function<void(void*)> constructor;
    std::function<void(void*)> destructor;
    std::function<void(void*, const void*)> copy;
    std::function<void(void*, void*)> move;
    std::function<int(void*, lua_State*)> to_luau;
};

class Registry;

class ComponentPool {
private:
    ComponentDescription m_description;
    std::vector<std::byte> m_data;
    std::vector<Entity> m_entities;
    std::vector<int32_t> m_sparse;

    friend Registry;
public:
    ComponentPool(const ComponentDescription &desc);
    ComponentPool(ComponentDescription &&desc);

    const void *Get(Entity entity) const;
    void *Get(Entity entity);
    template <typename T> const T& Get(Entity entity) const;
    template <typename T> T& Get(Entity entity);

    bool Has(Entity entity) const noexcept;

    void Add(Entity entity, const void *data = nullptr);
    template<typename T> void Add(Entity entity, const T& data);
    template<typename T> void Add(Entity entity, T&& data);    

    void Remove(Entity entity);

    const std::vector<Entity> &Entities() const noexcept;
};

class Query {
private:
    Signature m_signature;
    const ComponentPool *m_pool;
    const World::Registry *m_world;
public:
    Query(const Registry &reg, std::initializer_list<ComponentID> comp_ids);
    void Each(const std::function<void(Entity)> &callback);
    std::vector<Entity> Entities();
};

class IWorldSystem {
public:
    const std::string Name;
    virtual ~IWorldSystem() = default;
    virtual void Update(double dt) = 0;
};

class Registry final {
private:
    size_t max_entities = 1024;
    size_t max_components = 128;
    size_t max_systems = 64;

    struct system_desc {
        std::string name;
        int priority;
        std::unique_ptr<IWorldSystem> system;
    };

    std::vector<Signature> m_signatures;
    std::vector<Entity> m_entities;
    std::vector<Entity> m_entity_queue;
    std::vector<int32_t> m_index_array;
    std::vector<ComponentPool> m_components;
    std::vector<system_desc> m_systems;
    std::unordered_map<std::string, ComponentID> m_component_registry;

    friend Query::Query(const World::Registry &reg, std::initializer_list<ComponentID> comp_ids);
public:
    Registry();
    Entity NewEntity();
    void DeleteEntity(Entity entity);
    void Update(double dt);

    bool EntityHasComponent(Entity entity, ComponentID comp_id) const noexcept;
    bool EntityMatchesSignature(Entity entity, Signature signature) const noexcept;
    
    ComponentID RegisterComponent(const ComponentDescription &desc); 
    ComponentID RegisterComponent(ComponentDescription &&desc);
    template<typename T> ComponentID RegisterComponent(const std::function<int(void*,lua_State*)> &to_luau = [](void*,lua_State*){return 0;});

    const void *GetComponent(Entity entity, ComponentID comp_id) const;
    void *GetComponent(Entity entity, ComponentID comp_id);
    int GetComponent(Entity entity, ComponentID comp_id, lua_State *L);

    template<typename T> const T& GetComponent(Entity entity, ComponentID comp_id) const;
    template<typename T> T& GetComponent(Entity entity, ComponentID comp_id);

    void GiveComponent(Entity entity, ComponentID comp_id, const void *value = nullptr);
    template<typename T> void GiveComponent(Entity entity, ComponentID comp_id, const T& value);
    template<typename T> void GiveComponent(Entity entity, ComponentID comp_id, T&& value = T{});

    void RemoveComponent(Entity entity, ComponentID comp_id);

    template<typename T, typename... arg_types> void RegisterSystem(arg_types&&... args);
    bool IsComponent(ComponentID id) const noexcept;
    template<typename ...T> std::vector<Entity> NewQuery();

    void StoreComponentID(ComponentID id, std::string name);
    ComponentID GetComponentID(std::string name);
};


}

#endif
