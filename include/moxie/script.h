#ifndef __SCRIPT_H
#define __SCRIPT_H

#include <string>
#include <filesystem>
#include <vector>
#include <variant>
#include <moxie/world.hpp>

struct lua_State;

namespace Script {

struct ScriptContext {
    World::Registry *WorldRegistry;
};

class Scheduler {
private:
    enum class yield_cond {
        none,
        time
    };
    struct yield_info {
        enum yield_cond cond;
        union {
            struct {
                double remaining;
            };
        } data;
    };

    struct luau_script {
        lua_State *thread;
        yield_info yield;
    };

    lua_State *m_luau_thread;
    std::vector<luau_script> m_scripts;

    std::string compile_source(std::string&);
public:
    Scheduler();
    ~Scheduler();
    void LoadFile(std::filesystem::path);
    void RegisterScriptContext(ScriptContext&&);
    void Resume(double dt);
    void RegisterWorld(World::Registry *world);
};

}

#endif