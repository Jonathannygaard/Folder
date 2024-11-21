﻿#include "Input.h"
#include <iostream>
#include "../../Engine.h"
#include "../../Entity/Entity.h"
#include "../../Components/Component.h"
#include "../Window.h"

void Input::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void KeyBoardInput::processInput(GLFWwindow* window, Entity* player, ComponentManager* componentManager)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
    {
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position.x +=
            Engine::MainCamera.cameraSpeed * Engine::MainCamera.cameraFront.x * Engine::DeltaTime;
        
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position.z +=
            Engine::MainCamera.cameraSpeed * Engine::MainCamera.cameraFront.z * Engine::DeltaTime;
        
        Engine::MainCamera.setPlayerPos(componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position);
    }
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
    {
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position -=
            (Engine::MainCamera.cameraSpeed * glm::normalize(glm::cross(Engine::MainCamera.cameraFront, Engine::MainCamera.cameraUp))) * Engine::DeltaTime;
        
        Engine::MainCamera.setPlayerPos(componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position);
    }
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
    {
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position.x -=
            Engine::MainCamera.cameraSpeed * Engine::MainCamera.cameraFront.x * Engine::DeltaTime;
        
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position.z -=
            Engine::MainCamera.cameraSpeed * Engine::MainCamera.cameraFront.z * Engine::DeltaTime;
        
        Engine::MainCamera.setPlayerPos(componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position);
    }
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
    {
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position +=
            (Engine::MainCamera.cameraSpeed * glm::normalize(glm::cross(Engine::MainCamera.cameraFront, Engine::MainCamera.cameraUp))) * Engine::DeltaTime;
        
        Engine::MainCamera.setPlayerPos(componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position);
    }
    if(glfwGetKey(window, GLFW_KEY_E)==GLFW_PRESS)
    {
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position.y +=
            Engine::MainCamera.cameraSpeed * Engine::DeltaTime;
        Engine::MainCamera.setPlayerPos(componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position);
    }
    if(glfwGetKey(window, GLFW_KEY_Q)==GLFW_PRESS)
    {
        componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position.y -=
            Engine::MainCamera.cameraSpeed * Engine::DeltaTime;
        Engine::MainCamera.setPlayerPos(componentManager->GetComponentHandler<PositionComponent>()->GetComponent(player).Position);
    }
}

namespace MouseInput
{
    bool firstMouse = true;
    double lastX = 0.f;
    double lastY = 0.f;
    float yaw = -90.f;
    float pitch = 0.f;
    float fov = 45.f;

    Cube* player = nullptr;
}

void MouseInput::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(!Engine::MainCamera.CameraLock)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        auto xoffset = static_cast<float>(xpos - lastX);
        auto yoffset = static_cast<float>(lastY - ypos);
        lastX = xpos;
        lastY = ypos;
        const float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        Engine::MainCamera.cameraFront = glm::normalize(direction);
    }
}

void MouseInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}
