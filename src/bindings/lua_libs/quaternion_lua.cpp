#include <lua.h>
#include <lualib.h>
#include <cglm/cglm.h>
#include <lua_tags.h>
#include <iostream>

static int quat_from_axis_angle(lua_State *L) {
    const float *axis = luaL_checkvector(L, 1);
    double angle = luaL_checknumber(L, 2);

    auto *userdata = static_cast<float*>(lua_newuserdatatagged(L,
        sizeof(versor),
        static_cast<int>(UserdataTag::Quaternion)
    ));

    glm_quat(userdata, angle, axis[0], axis[1], axis[2]);

    return 1;
}

static int quat_new(lua_State *L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float z = luaL_checknumber(L, 3);
    float w = luaL_checknumber(L, 4);

    auto *userdata = static_cast<float*>(lua_newuserdatataggedwithmetatable(L,
        sizeof(versor),
        static_cast<int>(UserdataTag::Quaternion)
    ));

    glm_quat(userdata, w, x, y, z);

    return 1;
}

static int quat_print(lua_State *L) {
    auto *userdata = static_cast<float*>(lua_newuserdatataggedwithmetatable(L,
        1,
        static_cast<int>(UserdataTag::Quaternion)
    ));

    std::cout << userdata[0] << ", " << userdata[1] << ", " << userdata[2] << ", " << userdata[3] << std::endl;

    return 1;
}

static luaL_Reg quat_lib[] = {
    {"New", quat_new},
    {"FromAxisAngle", quat_from_axis_angle},
    {"Print", quat_print},
    {nullptr, nullptr}
};

void register_quat_lib(lua_State *L) {
    luaL_register(L, "Quaternion", quat_lib);
}