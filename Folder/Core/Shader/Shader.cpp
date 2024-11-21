#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

unsigned int Shader::Program;
ShaderSource Shader::ReadShaderFromFile(const char* filepathVert, const char* filepathFrag)
{
    ShaderSource currentSource;
    std::ifstream InFileVert(filepathVert);
    std::ifstream InFileFrag(filepathFrag);
    std::stringstream ssVert;
    std::stringstream ssFrag;
    if (InFileVert.is_open() && InFileFrag.is_open())
    {
        ssVert << InFileVert.rdbuf();
        ssFrag << InFileFrag.rdbuf();
        currentSource.VertexSource = ssVert.str();
        currentSource.FragmentSource = ssFrag.str();
        return currentSource;
    }
   
    return {};
}

void Shader::CreateProgram()
{
    ShaderSource source = ReadShaderFromFile("Core/Shader/VertexShader.shader", "Core/Shader/FragmentShader.shader");
    Program = glCreateProgram();
    const char* VertexSource = source.VertexSource.c_str();
    const char* FragmentSource = source.FragmentSource.c_str();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    int success;
    glShaderSource(vs, 1, &VertexSource, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    char infoLog[512];
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "VertexShader failed to compile\n" << infoLog << std::endl;
    }
    glShaderSource(fs, 1, &FragmentSource, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "FragmentShader failed to compile\n" << infoLog << std::endl;
    }
    glAttachShader(Program, vs);
    glAttachShader(Program, fs);
    glLinkProgram(Program);
    glDeleteShader(vs);
    glDeleteShader(fs);
}
