#include <lua.h>
#include <lualib.h>
#include <lua_tags.h>
#include <engine.hpp>
#include <render_system.hpp>
#include <cstring>
#include <iostream>

void camera_meta(lua_State *L) {
    luaL_newmetatable(L, "Camera");
    lua_pushliteral(L, "Camera");
    lua_setfield(L, -2, "__type");
    lua_pushliteral(L, "Locked");
    lua_setfield(L, -2, "__metatable");

    lua_pushcfunction(L, [](lua_State* L){
        auto *userdata = static_cast<World::ComponentHandle*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::CameraComponent)
        ));
        auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
        auto &world = engine->WorldRegistry;

        auto &camera = world.GetComponent<camera_data>(userdata->entity, userdata->comp_id);

        const char *key = luaL_checkstring(L, 2);

        if (strcmp(key, "Projection") == 0) {
            const char *projection = luaL_checkstring(L, 3);
            if (strcmp(projection, "Perspective") == 0) {
                camera.projection = projection_Perspective;
            } else if (strcmp(projection, "Orthographic") == 0) {
                camera.projection = projection_Orthographic;
            }
            return 0;
        } else if (strcmp(key, "YFov") == 0) {
            double fov = luaL_checknumber(L, 3);
            camera.frustrum.fieldofview = fov;
            return 0;
        } else if (strcmp(key, "AspectRatio") == 0) {
            double aspect = luaL_checknumber(L, 3);
            camera.frustrum.aspect = aspect;
            return 0;
        } else if (strcmp(key, "Near") == 0) {
            double near = luaL_checknumber(L, 3);
            camera.near = near;
            return 0;
        } else if (strcmp(key, "Far") == 0) {
            double far = luaL_checknumber(L, 3);
            camera.far = far;
            return 0;
        } else if (strcmp(key, "Left") == 0) {
            double left = luaL_checknumber(L, 3);
            camera.box.left = left;
            return 0;
        } else if (strcmp(key, "Right") == 0) {
            double right = luaL_checknumber(L, 3);
            camera.box.right = right;
            return 0;
        } else if (strcmp(key, "Top") == 0) {
            double top = luaL_checknumber(L, 3);
            camera.box.top = top;
            return 0;
        } else if (strcmp(key, "Bottom") == 0) {
            double bottom = luaL_checknumber(L, 3);
            camera.box.bottom = bottom;
            return 0;
        } else {
            luaL_error(L, "Unknown key");
        }

        return 0;
    }, "__newindex");
    lua_setfield(L, -2, "__newindex");
}