#ifndef __SCRIPT_H
#define __SCRIPT_H

#include <string>
#include <filesystem>

struct lua_State;

class LuauScript {
private:
    lua_State *thread = nullptr;
    std::string bytecode;
    std::string name;
public:
    LuauScript(std::filesystem::path);
    void InitThread(lua_State *main);
    int Load();
    void Run();
    int Status();
};

#endif