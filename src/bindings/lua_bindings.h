#ifndef LUA_BINDINGS
#define LUA_BINDINGS

struct lua_State;
namespace World { class Registry; };

int register_world(lua_State *L, World::Registry *world);

#endif