#ifndef INIT_META_H
#define INIT_META_H

#include <lua.h>
#include <lua_tags.h>

using LuauMetaFN = void(lua_State*);

inline void register_meta(lua_State *L, LuauMetaFN push_meta, UserdataTag tag) {
    push_meta(L);
    lua_setuserdatametatable(L, static_cast<int>(tag));
}

void transform_meta(lua_State *L);
void meshshape_meta(lua_State *L);
void meshrender_meta(lua_State *L);

#endif