#include <lua.h>
#include <lualib.h>
#include <engine.hpp>
#include <lua_tags.h>
#include <cstring>

void worldquery_meta(lua_State *L) {
    luaL_newmetatable(L, "WorldQuery");

    lua_pushliteral(L, "WorldQuery");
    lua_setfield(L, -2, "__type");
    lua_pushliteral(L, "Locked");
    lua_setfield(L, -2, "__metatable");

    lua_createtable(L, 0, 1);

    lua_pushcfunction(L, [](lua_State *L) {
        auto *userdata = static_cast<Signature*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::WorldQuery)
        ));

        int isfn = lua_gettop(L) >= 2 && lua_isfunction(L, 2);
        if (!isfn) {
            luaL_error(L, "Function expected for argument #2");
        }

        auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
        auto &world = engine->WorldRegistry;
        auto query = World::Query(world, *userdata);
        query.Each([L](Entity e){
            lua_pushvalue(L, 2);
            lua_pushinteger(L, e);
            lua_call(L, 1, 0);
        });
        
        return 0;
    }, "Each");

    lua_setfield(L, -2, "Each");
    lua_setfield(L, -2, "__index");
}