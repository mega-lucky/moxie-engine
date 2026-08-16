#ifndef __ECS_TYPES__
#define __ECS_TYPES__

#include <cstdint>
#include <bitset>
#include <vector>
#include <memory>
#include <functional>
#include <type_traits>

using Entity = uint32_t;
using ComponentID = uint32_t;
using Signature = std::bitset<128>;

namespace World {

class IWorldSystem;

class Registry {
private:
    size_t max_entities = 1024;
    size_t max_components = 128;
    size_t max_systems = 64;

    class IComponentPool;
    template<typename T> class ComponentPool;

    struct system_desc {
        std::string name;
        int priority;
        std::unique_ptr<IWorldSystem> system;
    };

    std::vector<Signature> m_signatures;
    std::vector<Entity> m_entities;
    std::vector<Entity> m_entity_queue;
    std::vector<int32_t> m_index_array;
    std::vector<std::unique_ptr<IComponentPool>> m_components;
    std::vector<system_desc> m_systems;
    std::bitset<128> m_component_registry;

    ComponentID next_component_id();
    template<typename T> ComponentID get_component_id();
    template<typename T> ComponentPool<T>* get_pool();
public:
    Registry();
    Entity NewEntity();
    void DeleteEntity(Entity entity);
    void Update();

    template<typename T> bool EntityHasComponent(Entity entity);
    template<typename T> void RegisterComponent();
    template<typename T> T& GetComponent(Entity entity);
    template<typename T> void GiveComponent(Entity entity);
    template<typename T> void GiveComponent(Entity entity, const T& value);
    template<typename T> void RemoveComponent(Entity entity);
    template<typename T> void RegisterSystem();
    template<typename T> void RegisterSystem(std::string name);
    template<typename T> void RegisterSystem(int priority);
    template<typename T> void RegisterSystem(std::string name, int priority);
    template<typename T> bool IsComponent();
    template<typename ...T> std::vector<Entity> NewQuery();
};

class Registry::IComponentPool {
public:
    virtual ~IComponentPool() = default;
    virtual void Remove(Entity entity) = 0;
    virtual bool Has(Entity entity) = 0;
    virtual size_t Size() = 0;
    virtual const std::vector<Entity>& Entities() = 0;
};

template<typename T>
class Registry::ComponentPool : public Registry::IComponentPool {
private:
    std::vector<T> m_data;
    std::vector<int32_t> m_sparse;
    std::vector<Entity> m_entities;
public:
    ComponentPool(size_t max_entities);
    void Add(Entity entity, const T& value);
    void Remove(Entity entity) override;
    bool Has(Entity entity) override;
    size_t Size() override;
    const std::vector<Entity>& Entities() override;
    T& Get(Entity entity);
};

class IWorldSystem {
public:
    const std::string Name;
    virtual ~IWorldSystem() = default;
    virtual void Update() = 0;
};

}

#endif