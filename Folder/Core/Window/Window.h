#pragma once

struct GLFWwindow;

class Window
{
public:
    static float Width;
    static float Height;

    static GLFWwindow* init();
};
