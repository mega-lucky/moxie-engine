#include <lua.h>
#include <lualib.h>
#include <engine.hpp>
#include <script.hpp>
#include <time.hpp>
#include <iostream>

static int time_wait(lua_State *L) {
    auto *engine = static_cast<Engine*>(lua_getthreaddata(lua_mainthread(L)));

    double time = lua_gettop(L) > 0
        ? luaL_checknumber(L, 1)
        : 0.0;

    auto *thread_info = static_cast<Script::ThreadInfo*>(
        lua_getthreaddata(L)
    );

    if (thread_info == nullptr) {
        luaL_error(L, "Unable to yield this thread");
    }

    auto& yield = thread_info->yield;
    
    yield.cond = Script::YieldCase::Time;
    yield.data.time.duration = time;
    yield.data.time.start_time = engine->Timer.GetTime();

    return lua_yield(L, 0);
}

static const luaL_Reg time_lib[] = {
    {"wait", time_wait},
    {nullptr, nullptr}
};

void register_time_lib(lua_State *L) {
    luaL_register(L, "time", time_lib);
}