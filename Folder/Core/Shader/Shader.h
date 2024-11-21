#pragma once
#include <string>

struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

namespace Shader
{
    extern unsigned int Program;
    ShaderSource ReadShaderFromFile(const char* filepathVert, const char* filepathFrag);
    void CreateProgram();
};
