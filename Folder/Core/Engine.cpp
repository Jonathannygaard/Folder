﻿#include "Engine.h"
#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Color.h"
#include "Shader/Shader.h"
#include "Window/Window.h"
#include "Window/Input/Input.h"
#include "GLFW/glfw3.h"

void Engine::Create()
{    
    //Creating player and enemy entities and adding them to the entities vector
    entities.emplace_back(0);
    //Creating point map
    entities.emplace_back(entities.size());

    //Creating Sphere entities
    for (int i = 0; i < AmountOfSpheres; i++)
    {
        entities.emplace_back(entities.size());
        spheres.push_back(entities.back());
    }
    
    //Making seed random
    srand(time(NULL));

    //Adding components to the player entity
    componentManager.AddComponent<MeshComponent>(&entities[0]);
    componentManager.AddComponent<PositionComponent>(&entities[0]);
    componentManager.AddComponent<MovementComponent>(&entities[0]);
    componentManager.AddComponent<CollisionComponent>(&entities[0]);
    componentManager.AddComponent<HealthComponent>(&entities[0]);
    
    //Setting up Sphere Entity
    for (Entity sphere: spheres)
    {
        componentManager.AddComponent<MeshComponent>(&sphere);
        componentManager.AddComponent<PositionComponent>(&sphere);
        componentManager.AddComponent<MovementComponent>(&sphere);
        componentManager.AddComponent<MassComponent>(&sphere);
        componentManager.AddComponent<CollisionComponent>(&sphere);
        componentManager.AddComponent<TrackingComponent>(&sphere);
        
        meshSystem.CreateSphereMesh(&sphere, 16, 16, 1.f, Color::Lavender);
        componentManager.GetComponentHandler<CollisionComponent>()->GetComponent(&sphere).Radius = 1.f;
        componentManager.GetComponentHandler<MassComponent>()->GetComponent(&sphere).Mass = 1.f;
        componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&sphere).Position =
             glm::vec3(rand()%100 + 50, rand()%20 + 10, rand()%100 + 50);
    }

    //Setting up Player entity
    meshSystem.CreateCubeMesh(&entities[0], Color::Cyan);
    componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entities[0]).Health = 5;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[0]).Position = glm::vec3(0,0,0);
    
    //Setting up floor entity
    componentManager.AddComponent<MeshComponent>(&entities[1]);
    componentManager.AddComponent<PositionComponent>(&entities[1]);
    xLength = meshSystem.CreateFloorMesh(&entities[1], TerrainResolution, isPointcloud);
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[1]).Position =
            glm::vec3(0.f,0.f,0.f);
}

void Engine::CreateSphere()
{
    entities.emplace_back(entities.size());
    spheres.push_back(entities.back());

    componentManager.AddComponent<MeshComponent>(&spheres.back());
    componentManager.AddComponent<PositionComponent>(&spheres.back());
    componentManager.AddComponent<MovementComponent>(&spheres.back());
    componentManager.AddComponent<MassComponent>(&spheres.back());
    componentManager.AddComponent<CollisionComponent>(&spheres.back());
    componentManager.AddComponent<TrackingComponent>(&spheres.back());
        
    meshSystem.CreateSphereMesh(&spheres.back(), 16, 16, 1.f, Color::Lavender);
    componentManager.GetComponentHandler<CollisionComponent>()->GetComponent(&spheres.back()).Radius = 1.f;
    componentManager.GetComponentHandler<MassComponent>()->GetComponent(&spheres.back()).Mass = 1.f;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&spheres.back()).Position =
        componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[0]).Position;
}

void Engine::setup()
{
    Window = Window::init();
    MainCamera.init();
    glLineWidth(10.f);
    
    Create();
}

void Engine::Draw()
{
    for(Entity entity : entities)
    {
        if (entity.ID == 0)
        {
            continue;
        }        
        meshSystem.DrawMesh(&entity);
    }
}

void Engine::update()
{
    collisionSystem.UpdatePosition(&entities[0]);
    
    for (Entity s: spheres)
    {
        collisionSystem.UpdatePosition(&s);
        for(Entity entity : spheres)
        {
            if(s.ID == entity.ID)
            {
                continue;
            }
            collisionSystem.CheckSphereCollision(&s, &entity);
        }
        if (!collisionSystem.BarycentricCoordinates(&entities[1], &s, TerrainResolution, xLength))
        {
            movementSystem.Gravity(&s);
        }
        movementSystem.MoveEntity(&s);
    }
    if (tracktimer > trackinterval)
    {
        for (Entity s: spheres)
        {
            trackingsystem.TrackSphere(&s, &meshSystem);        
            tracktimer = 0;
        }
    }
    tracktimer += DeltaTime;
    
    Draw();
    MainCamera.OrbitCamera();
}

void Engine::run()
{
    setup();
    float FirstFrame = 0.0f;
    glm::vec3 color(Color::Cyan);
    while(!glfwWindowShouldClose(Window))
    {
        const auto CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - FirstFrame;
        FirstFrame = CurrentFrame;
        
        glClearColor(color.x, color.y, color.z, 1.f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Shader::Program);
		  
        update();
		  
        glUniformMatrix4fv(MainCamera.projectionLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getProjection(Window::Width, Window::Height)));
        glUniformMatrix4fv(MainCamera.viewLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getView()));

        KeyBoardInput::processInput(Window, entities, &componentManager, this);
    
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwTerminate();
}

