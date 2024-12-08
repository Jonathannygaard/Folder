#include "Lua.h"
#include <string>
#include "../Engine.h"

int CreateSphere(lua_State* L)
{
    float x = lua_tonumber(L, 1);
    float y = lua_tonumber(L, 2);
    float z = lua_tonumber(L, 3);
    Engine::get().CreateSphere({x,y,z});
    return 0;
}

int GetTotalTime(lua_State* L)
{
    lua_pushnumber(L, Engine::get().totaltime);
    return 1;
}

int GetDeltatime(lua_State* L)
{
    lua_pushnumber(L, Engine::DeltaTime);
    return 1;
}

void Lua::setup()
{
    // initialize Lua interpreter
    L = luaL_newstate();

    // load Lua base libraries (print / math / etc)
    luaL_openlibs(L);
}

void Lua::runInterpreter()
{
    char inputBuffer[500];
    while (true)
    {
        printf(">");
        fgets(inputBuffer, sizeof inputBuffer, stdin);
        luaL_dostring(L, inputBuffer);
    }
}

void Lua::RegisterFunctions()
{
    lua_register(L, "CreateSphere", CreateSphere);
    lua_register(L, "GetTotalTime", GetTotalTime);
    lua_register(L, "GetDeltatime", GetDeltatime);
}

void Lua::DoFile()
{
    std::string path = "Core/Lua/myFile.lua";
    luaL_dofile(L, path.c_str());
}
