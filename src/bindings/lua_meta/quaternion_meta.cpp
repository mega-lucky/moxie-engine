#include <lua.h>
#include <lualib.h>
#include <cglm/cglm.h>
#include <cglm/quat.h>
#include <lua_tags.h>
#include <cstring>

void quaternion_meta(lua_State *L) {
    luaL_newmetatable(L, "Quaternion");

    lua_pushliteral(L, "Quaternion");
    lua_setfield(L, -2, "__type");
    lua_pushliteral(L, "Locked");
    lua_setfield(L, -2, "__metatable");

    lua_pushcfunction(L, [](lua_State *L){
        auto *userdata = static_cast<float*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::Quaternion)
        ));
        const auto *k = luaL_checkstring(L, 2);

        if (strcmp(k, "Axis") == 0) {
            vec3 axis;
            glm_quat_axis(userdata, axis);
            lua_pushvector(L, axis[0], axis[1], axis[2]);
            return 1;
        } else if (strcmp(k, "Angle") == 0) {
            lua_pushnumber(L, glm_quat_angle(userdata));
            return 1;
        }
        return 0;
    }, "__index");
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, [](lua_State *L){
        auto *userdata = static_cast<float*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::Quaternion)
        ));

        int type = lua_type(L, 2);
        switch (type) {
            case LUA_TUSERDATA: {
                auto *other = static_cast<float*>(luaL_checkudatatagged(
                    L,
                    2,
                    static_cast<int>(UserdataTag::Quaternion)
                ));

                auto *result = static_cast<float*>(lua_newuserdatataggedwithmetatable(L,
                    sizeof(versor),
                    static_cast<int>(UserdataTag::Quaternion)
                ));

                glm_quat_mul(userdata, other, result);
                return 1;
            }
            case LUA_TVECTOR: {
                const float *other = lua_tovector(L, 2);
                vec3 other_vec;
                other_vec[0] = other[0];
                other_vec[1] = other[1];
                other_vec[2] = other[2];
                vec3 result;
                glm_quat_rotatev(userdata, other_vec, result);
                lua_pushvector(L, result[0], result[1], result[2]);
                return 1;
            }
        }
        
        return 0;
    }, "__mul");
    lua_setfield(L, -2, "__mul");
}