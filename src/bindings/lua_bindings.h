#ifndef LUA_BINDINGS
#define LUA_BINDINGS

#include <engine.hpp>

struct lua_State;

int register_world(lua_State *L);
void register_time_lib(lua_State *L);
void register_inputs_lib(lua_State *L);
void register_quat_lib(lua_State *L);

#endif