#include "moxie/script.h"
#include "lua.h"
#include "lualib.h"
#include "luacode.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <lua_bindings.h>

std::string Script::Scheduler::compile_source(std::string &content) {
    size_t size;

    char *compiled = luau_compile(
        content.data(),
        content.size(),
        nullptr,
        &size
    );

    if (compiled == nullptr) {
        throw std::runtime_error("Failed to compile source code");
    }
    std::string bytecode = std::string(compiled, size);
    free(compiled);

    return bytecode;
}

Script::Scheduler::Scheduler() : m_luau_thread(luaL_newstate()) {
    luaL_openlibs(m_luau_thread);
}
Script::Scheduler::~Scheduler() {
    lua_close(m_luau_thread);
    m_luau_thread = nullptr;
}

void Script::Scheduler::LoadFile(std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    size_t size = 0;

    std::string bytecode = compile_source(content);
    std::string name = path.filename().string();

    lua_State *new_thread = lua_newthread(m_luau_thread);
    lua_ref(m_luau_thread, LUA_REGISTRYINDEX);

    int load_state = luau_load(
        new_thread,
        ("@" + name).c_str(),
        bytecode.c_str(),
        size,
        0
    );

    if (load_state != 0) {
        const char *message = lua_tostring(new_thread, -1);
        throw std::runtime_error("Failed to load script " + name + ":\n\t" + message);
    }

    luau_script &container = m_scripts.emplace_back();
    container.thread = new_thread;
    container.yield.cond = yield_cond::time;
    container.yield.data.remaining = 100.0;
}

void Script::Scheduler::Resume(double dt) {
    for (auto &script : m_scripts) {
        int status = lua_status(script.thread);
        if (status == LUA_YIELD) {
            yield_info &yield = script.yield;
            switch (yield.cond) {
                case yield_cond::time: {
                    yield.data.remaining -= dt;
                    if (yield.data.remaining > 0.0) {
                        continue;
                    }
                    break;
                }
            }
        }

        status = lua_resume(script.thread, nullptr, 0);

        if (status >= 2) {
            const char *err_msg = lua_tostring(script.thread, -1);
            std::cerr << "Luau Runtime Error:\n\t" << err_msg << std::endl;
        }

        if (
            status == LUA_OK && lua_gettop(script.thread) == 0 ||
            status >= 2
        ) {
            std::swap(script, m_scripts.back());
            m_scripts.pop_back();
        }
    }
}

void Script::Scheduler::RegisterWorld(World::Registry *world) {
    register_world(m_luau_thread, world);
}