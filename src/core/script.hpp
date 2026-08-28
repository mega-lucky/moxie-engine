#ifndef __SCRIPT_H
#define __SCRIPT_H

#include <string>
#include <filesystem>
#include <vector>
#include <variant>
#include <world.hpp>

struct lua_State;
class Engine;

namespace Script {

enum class YieldCase {
    None,
    Time
};
struct YieldInfo {
    enum YieldCase cond;
    union {
        struct {
            double start_time;
            double duration;
        } time;
    } data;
};

struct ThreadInfo {
    int ref;
    YieldInfo yield;
};

class Scheduler {
private:
    lua_State *m_mainthread;
    std::vector<lua_State*> m_threads;

    std::string compile_source(std::string&);
    void load_bytecode(lua_State *thread, std::string bytecode, std::string name);
    lua_State *new_thread();
    void register_thread();
    void kill_thread(lua_State *thread);
public:
    Scheduler(Engine &engine);
    ~Scheduler();
    void LoadFile(std::filesystem::path);
    void Resume(double dt);
};

}

#endif