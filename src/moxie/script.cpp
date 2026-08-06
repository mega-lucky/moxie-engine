#include "moxie/script.h"
#include "lua.h"
#include "lualib.h"
#include "luacode.h"
#include <fstream>
#include <sstream>

LuauScript::LuauScript(std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    size_t size = 0;

    char *compiled = luau_compile(
        content.data(),
        content.size(),
        nullptr,
        &size
    );

    if (compiled != nullptr) {
        bytecode = std::string(compiled, size);
        free(compiled);
    }
}

void LuauScript::InitThread(lua_State *main) {
    if (thread != nullptr) {
        lua_close(thread);
    }
    thread = lua_newthread(main);
}

int LuauScript::Load() {
    return luau_load(
        thread,
        name.c_str(),
        bytecode.data(),
        bytecode.size(),
        0
    );
}

void LuauScript::Run() {
    int nresults = 0;
    int status = lua_resume(thread, nullptr, 0);
}

int LuauScript::Status() {
    return lua_status(thread);
}