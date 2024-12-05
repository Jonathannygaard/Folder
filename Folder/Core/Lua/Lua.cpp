#include "Lua.h"
#include "../Engine.h"

int CreateSphere(lua_State* L)
{
    
    
    return 1;
}

//  C Functions that lua can call
int cAdd(lua_State* L)
{
    // Step 1:  extract the parameters from the lua stack:
    double n1 = lua_tonumber(L, 1);
    double n2 = lua_tonumber(L, 2);

    //  Step 2:  Do the actual calculation.  Normally, this will be more interesting than a single sum!
    double sum = n1 + n2;

    // Step 3:  Push the result on the lua stack. 
    lua_pushnumber(L, sum);

    // Return the number of arguments we pushed onto the stack (that is, the number of return values this
    // function has
    return 1;
}

int average(lua_State* L)
{
    // Get the number of parameters
    int n = lua_gettop(L);

    double sum = 0;
    int i;

    // loop through each argument, adding them up
    for (i = 1; i <= n; i++)
    {
        sum += lua_tonumber(L, i);
    }

    // push the average on the lua stack
    lua_pushnumber(L, sum / n);

    // push the sum on the lua stack
    lua_pushnumber(L, sum);

    // return the number of results we pushed on the stack
    return 2;
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
    lua_register(L, "cAdd", cAdd);
    lua_register(L, "average", average);
}

void Lua::DoFile()
{
    luaL_dofile(L, "C:/Visualisering og simulering/Folder/Folder/myFile.lua");
}
