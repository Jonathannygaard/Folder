#include "Window.h"
#include <glad/glad.h>
#include <iostream>
#include "GLFW/glfw3.h"
#include "Input/Input.h"
#include "../Shader/Shader.h"

float Window::Width = 1920.f;
float Window::Height = 1080.f;

GLFWwindow* Window::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;
    window = glfwCreateWindow((int)Width,(int)Height,"Compulsory 2",NULL,NULL);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    glViewport(0,0,Width,Height);
    glEnable(GL_DEPTH_TEST);
    Shader::CreateProgram();
    glfwSetFramebufferSizeCallback(window,Input::framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, MouseInput::mouse_button_callback);
    glfwSetCursorPosCallback(window, MouseInput::mouse_callback);
    return window;
}
