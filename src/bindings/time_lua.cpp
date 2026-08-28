#include <lua.h>
#include <lualib.h>
#include <script.hpp>
#include <iostream>


static int time_wait(lua_State *L) {
    double time = luaL_checknumber(L, 1);

    auto *thread_info = static_cast<Script::ThreadInfo*>(
        lua_getthreaddata(L)
    );

    if (thread_info == nullptr) {
        luaL_error(L, "Unable to yield this thread");
    }

    auto& yield = thread_info->yield;
    
    yield.cond = Script::YieldCase::Time;
    yield.data.time.duration = time;
    yield.data.time.start_time = g_timer->GetTime();

    return lua_yield(L, 0);
}

static const luaL_Reg time_lib[] = {
    {"wait", time_wait},
    {nullptr, nullptr}
};

void register_time_lib(lua_State *L, const Time &timer) {
    g_timer = &timer;
    luaL_register(L, "time", time_lib);
}