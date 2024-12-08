#pragma once

extern "C"
{
#include "lua54/include/lua.h"
#include "lua54/include/lauxlib.h"
#include "lua54/include/lualib.h"
}

class Lua
{
public:
    lua_State* L;
    
    void setup();
    void runInterpreter();
    void RegisterFunctions();
    void DoFile();
};
