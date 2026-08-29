#include <lua.h>
#include <lualib.h>
#include <cstring>
#include <lua_tags.h>
#include <engine.hpp>
#include <render_system.hpp>

void meshrender_meta(lua_State *L) {
    luaL_newmetatable(L, "MeshRender");

    lua_pushliteral(L, "MeshRender");
    lua_setfield(L, -2, "__type");
    lua_pushliteral(L, "Locked");
    lua_setfield(L, -2, "__metatable");

    lua_pushcfunction(L, [](lua_State *L){
        auto *handle = static_cast<World::ComponentHandle*>(luaL_checkudatatagged(
            L,
            1,
            static_cast<int>(UserdataTag::MeshRenderComponent)
        ));
        const char *k = luaL_checkstring(L, 2);

        auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
        auto &data = engine->WorldRegistry.GetComponent<mesh_renderer>(handle->entity, handle->comp_id);

        if (strcmp(k, "Material") == 0) {
            const char *name = luaL_checkstring(L, 3);
            data.material = engine->Assets.materials.at(name);
        }

        return 0;
    }, "__newindex");

    lua_setfield(L, -2, "__newindex");
}