#include <lua.h>
#include <lualib.h>
#include <engine.hpp>

int is_key_down(lua_State *L) {
    auto key = reinterpret_cast<uintptr_t>(lua_tolightuserdata(L, 1));
    auto key_enum = static_cast<Input::KeyInput>(key);
    if (key_enum == Input::KeyInput::Unknown) {
        luaL_error(L, "Invalid argument #1 expected a KeyInput");
    }

    auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
    auto &inputs = engine->InputManager;

    lua_pushboolean(L, inputs.IsKeyDown(key_enum));

    return 1;
}

int is_mouse_button_down(lua_State *L) {
    auto btn = reinterpret_cast<uintptr_t>(lua_tolightuserdata(L, 1));
    auto btn_enum = static_cast<Input::MouseInput>(btn);
    if (btn_enum == Input::MouseInput::Unknown) {
        luaL_error(L, "Invalid argument #1 expected a MouseInput");
    }

    auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));
    auto &inputs = engine->InputManager;

    lua_pushboolean(L, inputs.IsMouseButtonDown(btn_enum));

    return 1;
}

luaL_Reg inputs_lib[] = {
    {"IsKeyDown", is_key_down},
    {"IsMouseButtonDown", is_mouse_button_down},
    {nullptr, nullptr}
};

void register_inputs_lib(lua_State *L) {
    luaL_register(L, "Input", inputs_lib);

    lua_createtable(L, 0, static_cast<int>(Input::KeyInput::Count));
    #define X(n) \
        lua_pushlightuserdata(L, reinterpret_cast<void*>( \
            static_cast<uintptr_t>(Input::KeyInput::n) \
        )); \
        lua_setfield(L, -2, #n);
    KEY_BUTTONS_MACRO
    #undef X
    lua_setfield(L, -2, "KeyInput");

    lua_createtable(L, 0, static_cast<int>(Input::MouseInput::Count));
    #define X(n) \
        lua_pushlightuserdata(L, reinterpret_cast<void*>( \
            static_cast<uintptr_t>(Input::MouseInput::n) \
        )); \
        lua_setfield(L, -2, #n);
    MOUSE_BUTTONS_MACRO
    #undef X
    lua_setfield(L, -2, "MouseInput");
}