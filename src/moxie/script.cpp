#include "moxie/script.h"
#include "lua.h"
#include "lualib.h"
#include "luacode.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <lua_bindings.h>

std::string Script::Scheduler::compile_source(std::string& content) {
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

void Script::Scheduler::register_bytecode(std::string bytecode, std::string name) {
    lua_State *new_thread = lua_newthread(m_mainthread);
    int ref = lua_ref(m_mainthread, -1);
    lua_pop(m_mainthread, 1);

    int load_state = luau_load(
        new_thread,
        name.c_str(),
        bytecode.c_str(),
        bytecode.size(),
        0
    );

    if (load_state != 0) {
        const char *message = lua_tostring(new_thread, -1);
        throw std::runtime_error("Failed to load script " + name + ":\n\t" + message);
    }
    
    auto *thread_info = new Script::ThreadInfo;
    thread_info->ref = ref;
    lua_setthreaddata(new_thread, thread_info);

    m_threads.push_back(new_thread);
}

void Script::Scheduler::kill_thread(lua_State *thread) {
    auto *thread_data = static_cast<ThreadInfo*>(
        lua_getthreaddata(thread)
    );

    int ref = thread_data->ref;
    lua_setthreaddata(thread, nullptr);
    delete thread_data;

    lua_unref(m_mainthread, ref);
}

Script::Scheduler::Scheduler() : m_mainthread(luaL_newstate()) {
    luaL_openlibs(m_mainthread);
    register_time_lib(m_mainthread);
}
Script::Scheduler::~Scheduler() {
    lua_close(m_mainthread);
    m_mainthread = nullptr;
}

void Script::Scheduler::LoadFile(std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File not found.");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::string bytecode = compile_source(content);
    std::string name = ("@" + path.filename().string());

    register_bytecode(bytecode, name);
}

void Script::Scheduler::Resume(double dt) {
    for (size_t i = 0; i < m_threads.size(); ) {
        auto& thread = m_threads[i];
        bool resume = false;
        auto *thread_data = static_cast<ThreadInfo*>(
            lua_getthreaddata(thread)
        );

        int status = lua_status(thread);
        if (status == LUA_YIELD) {
            auto &yield = thread_data->yield;
            switch (yield.cond) {
                case YieldCase::Time: {
                    yield.data.remaining -= dt;
                    
                    if (yield.data.remaining <= 0.0) {
                        resume = true;
                    }
                    break;
                }
                default: {
                    resume = false;
                }
            }
        }

        if (status == LUA_OK && lua_gettop(thread) > 0) {
            resume = true;
        }

        if (resume) {
            status = lua_resume(thread, nullptr, 0);

            if (status >= 2) {
                const char *err_msg = lua_tostring(thread, -1);
                std::cerr << "Luau Runtime Error:\n\t" << err_msg << std::endl;
            }
        }

        if (
            status == LUA_OK && !resume ||
            status >= 2
        ) {
            kill_thread(thread);
            std::swap(thread, m_threads.back());
            m_threads.pop_back();
        } else {
            i ++;
        }
    }
}

void Script::Scheduler::RegisterWorld(World::Registry *world) {
    register_world(m_mainthread, world);
}