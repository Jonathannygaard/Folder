#include <iostream>
#include <ostream>
#include "Core/Engine.h"

int main(int argc, char* argv[])
{
    Engine engine;
    try
    {
        engine.run(); 
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
