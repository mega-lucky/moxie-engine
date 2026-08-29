#include <lua.h>
#include <lualib.h>
#include <iostream>
#include <engine.hpp>
#include <cstring>
#include <lua_tags.h>

static int world_new_entity_luau(lua_State *L) {
    auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
    Entity new_entity = engine->WorldRegistry.NewEntity();
    lua_pushinteger(L, new_entity);
    return 1;
}

static int world_del_entity_luau(lua_State *L) {
    auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
    auto entity = static_cast<Entity>(luaL_checkinteger(L, 1));
    engine->WorldRegistry.DeleteEntity(entity);
    return 0;
}

static int world_get_component(lua_State *L) {
    auto entity = static_cast<Entity>(luaL_checkinteger(L, 1));
    auto comp_id = static_cast<ComponentID>(luaL_checkinteger(L, 2));

    return g_world->GetComponent(entity, comp_id, L);
}

static luaL_Reg world_lib[] = {
    {"NewEntity", world_new_entity_luau},
    {"DeleteEntity", world_del_entity_luau},
    {"GetComponent", world_get_component},
    {nullptr, nullptr}
};

int register_world(lua_State *L) {
    lua_newtable(L);
    
    for (auto &reg : world_lib) {
        if (!reg.name || !reg.func) {
            break;
        }
        lua_pushcfunction(L, reg.func, nullptr);
        lua_setfield(L, -2, reg.name);
    }

    lua_setglobal(L, "World");

    return 1;
}