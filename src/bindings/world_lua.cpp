#include <lua.h>
#include <lualib.h>
#include <iostream>
#include <moxie/world.hpp>
#include <cstring>

static World::Registry *g_world = nullptr;

static int world_new_entity_luau(lua_State *L) {
    Entity new_entity = g_world->NewEntity();
    lua_pushinteger(L, new_entity);
    return 1;
}

static int world_del_entity_luau(lua_State *L) {
    auto entity = static_cast<Entity>(lua_tointeger(L, 1));
    g_world->DeleteEntity(entity);
    return 0;
}

static luaL_Reg world_lib[] = {
    {"NewEntity", world_new_entity_luau},
    {"DeleteEntity", world_del_entity_luau},
    {nullptr, nullptr}
};

int register_world(lua_State *L, World::Registry *world) {
    g_world = world;
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