#pragma once
#include <GLFW/glfw3.h>

class ComponentManager;
class Entity;
class Cube;
class Camera;
struct GLFWwindow;

class Input
{
public:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

class KeyBoardInput
{
public:
    static void processInput(GLFWwindow* window, Entity* player, ComponentManager* componentManager);
};

namespace MouseInput
{
    extern bool firstMouse;
    extern double lastX;
    extern double lastY;

    extern float yaw;
    extern float pitch;

    extern float fov;

    extern Cube* player;

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};