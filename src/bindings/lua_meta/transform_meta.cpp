#include <lua.h>
#include <lualib.h>
#include <transform.hpp>
#include <cstring>
#include <algorithm>
#include <lua_tags.h>
#include <engine.hpp>

void transform_meta(lua_State *L) {
    luaL_newmetatable(L, "Transform");

    lua_pushliteral(L, "Transform");
    lua_setfield(L, -2, "__type");
    lua_pushliteral(L, "Locked");
    lua_setfield(L, -2, "__metatable");

    lua_pushcfunction(L, [](lua_State *L){
        auto *handle = static_cast<World::ComponentHandle*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::TransformComponent)
        ));
        const char *k = luaL_checkstring(L, 2);

        auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
        transform &data = engine->WorldRegistry.GetComponent<transform>(handle->entity, handle->comp_id);

        if (strcmp(k, "Position") == 0) {
            lua_pushvector(L, data.position[0], data.position[1], data.position[2]);
            return 1;
        } else if (strcmp(k, "Scale") == 0) {
            lua_pushvector(L, data.scale[0], data.scale[1], data.scale[2]);
            return 1;
        } else if (strcmp(k, "Rotation") == 0) {
            auto *quat = static_cast<float*>(lua_newuserdatataggedwithmetatable(
                L,
                sizeof(versor),
                static_cast<int>(UserdataTag::Quaternion)
            ));
            
            memcpy(quat, data.rotation, sizeof(versor));
            return 1;
        }

        return 0;
    }, "__index");

    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, [](lua_State *L){
        auto *handle = static_cast<World::ComponentHandle*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::TransformComponent)
        ));
        const char *k = luaL_checkstring(L, 2);

        auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
        transform &data = engine->WorldRegistry.GetComponent<transform>(handle->entity, handle->comp_id);

        if (strcmp(k, "Position") == 0) {
            const float *vec = luaL_checkvector(L, 3);
            std::copy(vec, vec + 3, data.position);
            return 0;
        } else if (strcmp(k, "Scale") == 0) {
            const float *vec = luaL_checkvector(L, 3);
            std::copy(vec, vec + 3, data.scale);
            return 0;
        } else if (strcmp(k, "Rotation") == 0) {
            auto *quat = static_cast<float*>(luaL_checkudatatagged(
                L,
                3,
                static_cast<int>(UserdataTag::Quaternion)
            ));
            
            memcpy(data.rotation, quat, sizeof(versor));
            return 0;
        }

        return 0;
    }, "__newindex");

    lua_setfield(L, -2, "__newindex");
}