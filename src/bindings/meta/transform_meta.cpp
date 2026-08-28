#include <lua.h>
#include <lualib.h>
#include <transform.hpp>
#include <cstring>
#include <algorithm>

void init_transform_meta(lua_State *L) {
    luaL_newmetatable(L, "Transform");

    lua_pushliteral(L, "Transform");
    lua_setfield(L, -2, "__type");
    lua_pushliteral(L, "Locked");
    lua_setfield(L, -2, "__metatable");

    lua_pushcfunction(L, [](lua_State *L){
        auto *t = static_cast<transform**>(luaL_checkudata(L, 1, "Transform"));
        const char *k = luaL_checkstring(L, 2);

        if (strcmp(k, "Position") == 0) {
            lua_pushvector(L, (*t)->position[0], (*t)->position[1], (*t)->position[2]);
            return 1;
        } else if (strcmp(k, "Scale") == 0) {
            lua_pushvector(L, (*t)->scale[0], (*t)->scale[1], (*t)->scale[2]);
            return 1;
        } else if (strcmp(k, "Rotation") == 0) {
            lua_createtable(L, 4, 0);
            for (int i = 0; i < 4; i ++) {
                lua_pushnumber(L, (*t)->rotation[i]);
                lua_rawseti(L, -2, i+1);
            }
            return 1;
        }

        return 0;
    }, "__index");

    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, [](lua_State *L){
        auto *t = static_cast<transform**>(luaL_checkudata(L, 1, "Transform"));
        const char *k = luaL_checkstring(L, 2);

        if (strcmp(k, "Position") == 0) {
            const float *vec = luaL_checkvector(L, 3);
            std::copy(vec, vec + 3, (*t)->position);
            return 0;
        } else if (strcmp(k, "Scale") == 0) {
            const float *vec = luaL_checkvector(L, 3);
            std::copy(vec, vec + 3, (*t)->scale);
            return 0;
        } else if (strcmp(k, "Rotation") == 0) {
            lua_createtable(L, 4, 0);
            for (int i = 0; i < 4; i ++) {
                lua_pushnumber(L, (*t)->rotation[i]);
                lua_rawseti(L, -2, i+1);
            }
            return 1;
        }

        return 0;
    }, "__newindex");

    lua_setfield(L, -2, "__newindex");
}