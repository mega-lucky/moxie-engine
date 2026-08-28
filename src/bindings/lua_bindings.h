#ifndef LUA_BINDINGS
#define LUA_BINDINGS

#include <engine.hpp>

struct lua_State;

int register_world(lua_State *L, World::Registry &world);
void register_time_lib(lua_State *L, const Time &timer);

#endif