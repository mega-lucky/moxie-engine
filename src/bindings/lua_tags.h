#ifndef LUA_UD_TAGS
#define LUA_UD_TAGS

#include <world.hpp>

enum class UserdataTag : int {
    None = 0,
    WorldQuery,
    ComponentBase,
    TransformComponent,
    MeshShapeComponent,
    MeshRenderComponent,
};

static constexpr UserdataTag GetComponentTag(ComponentID comp_id) {
    auto tag = static_cast<int>(UserdataTag::ComponentBase) + comp_id + 1;
    return static_cast<UserdataTag>(tag);
}

#endif