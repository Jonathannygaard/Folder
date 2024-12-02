#pragma once
#include <vector>
#include <GLFW/glfw3.h>

class Engine;
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
    static void processInput(GLFWwindow* window, std::vector<Entity>& entities, ComponentManager* componentManager, Engine* engine);
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