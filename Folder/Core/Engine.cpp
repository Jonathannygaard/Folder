#include "Engine.h"

#include <dinput.h>
#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Color.h"
#include "Shader/Shader.h"
#include "Window/Window.h"
#include "Window/Input/Input.h"
#include "GLFW/glfw3.h"

Engine& Engine::get()
{
    if (nullptr == instance)
    {
        instance = new Engine;
    }

    return *instance;
}

void Engine::Create()
{    
    //Creating player and enemy entities and adding them to the entities vector
    entities.emplace_back(0);
    //Creating point map
    entities.emplace_back(entities.size());

    //Create floor entity
    entities.emplace_back(entities.size());
    
    //Making seed random
    srand(time(NULL));

    //Adding components to the player entity
    componentManager.AddComponent<MeshComponent>(&entities[0]);
    componentManager.AddComponent<PositionComponent>(&entities[0]);
    componentManager.AddComponent<MovementComponent>(&entities[0]);
    componentManager.AddComponent<CollisionComponent>(&entities[0]);
    componentManager.AddComponent<HealthComponent>(&entities[0]);
    
    //Setting up Sphere Entity
    for (int i = 0; i < AmountOfSpheres; i++)
    {
        CreateSphere(glm::vec3(rand()%100 + 50, rand()%20 + 10, rand()%100 + 50));
    }

    //Setting up Player entity
    color_component.Colors.emplace_back(Color::Cyan);
    meshSystem.CreateCubeMesh(&entities[0], color_component.Colors.back());
    componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entities[0]).Health = 5;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[0]).Position = glm::vec3(0,0,0);
    
    //Setting up floor entity
    componentManager.AddComponent<MeshComponent>(&entities[1]);
    componentManager.AddComponent<PositionComponent>(&entities[1]);
    xLength = meshSystem.CreateFloorMesh(&entities[1], TerrainResolution, isPointcloud);
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[1]).Position =
            glm::vec3(0.f,0.f,0.f);

    //Setting up floor entity
    componentManager.AddComponent<MeshComponent>(&entities[2]);
    componentManager.AddComponent<PositionComponent>(&entities[2]);
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[2]).Position =
            glm::vec3(-50.f,0.f,-50.f);
    color_component.Colors.emplace_back(Color::Green);
    meshSystem.CreateCubeMesh(&entities[2], color_component.Colors.back());
    componentManager.GetComponentHandler<MeshComponent>()->GetComponent(&entities[2]).Scale = glm::vec3(100, 1, 100);



    //Setting up particle system
    particleSystem.CreateParticle();
}

void Engine::CreateSphere(glm::vec3 position)
{
    entities.emplace_back(entities.size());
    spheres.push_back(entities.back());

    componentManager.AddComponent<MeshComponent>(&spheres.back());
    componentManager.AddComponent<PositionComponent>(&spheres.back());
    componentManager.AddComponent<MovementComponent>(&spheres.back());
    componentManager.AddComponent<MassComponent>(&spheres.back());
    componentManager.AddComponent<CollisionComponent>(&spheres.back());
    componentManager.AddComponent<TrackingComponent>(&spheres.back());
    color_component.Colors.emplace_back(Color::Lavender);
    
    meshSystem.CreateSphereMesh(&spheres.back(), 16, 16, 1.f, color_component.Colors.back());
    componentManager.GetComponentHandler<CollisionComponent>()->GetComponent(&spheres.back()).Radius = 1.f;
    componentManager.GetComponentHandler<MassComponent>()->GetComponent(&spheres.back()).Mass = 1.f;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&spheres.back()).Position = position;
}

void Engine::setup()
{
    lua.setup();
    lua.RegisterFunctions();
    
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
    particleSystem.DrawParticles();
}

void Engine::update()
{
    lua.DoFile();
    
    collisionSystem.UpdatePosition(entities[0]);
    
    for (Entity s: spheres)
    {
        collisionSystem.UpdatePosition(s);
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
    
    if (Particletimer > Particleinterval)
    {
        particleSystem.Emit(glm::vec3(rand()%100 - 50, rand()%20 + 10, rand()%100 - 150));
        Particletimer = 0;
    }
    Particletimer += DeltaTime;    
    
    particleSystem.Update();
    
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
        totaltime += DeltaTime;
        
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

