#include "Engine.h"
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
    for(int AmountOfEnemies = 0; AmountOfEnemies < 0; AmountOfEnemies++)
    {
        enemies.emplace_back(entities.size());
        entities.emplace_back(entities.size());
    }
    for(int AmountOfPickups = 0; AmountOfPickups < 0; AmountOfPickups++)
    {
        pickups.emplace_back(entities.size());
        entities.emplace_back(entities.size());
    }

    //Creating point map
    entities.emplace_back(entities.size());

    //Making seed random
    srand(time(NULL));

    //Adding components to the player entity
    componentManager.AddComponent<MeshComponent>(&entities[0]);
    componentManager.AddComponent<PositionComponent>(&entities[0]);
    componentManager.AddComponent<MovementComponent>(&entities[0]);
    componentManager.AddComponent<CollisionComponent>(&entities[0]);
    componentManager.AddComponent<HealthComponent>(&entities[0]);

    meshSystem.CreateCubeMesh(&entities[0], Color::Cyan);
    componentManager.GetComponentHandler<MovementComponent>()->GetComponent(&entities[0]).Speed = 5.f;
    componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entities[0]).Health = 5;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[0]).Position = glm::vec3(0,0,0);

    //Adding components to the enemy entities
    for(Entity entity : enemies)
    {
        componentManager.AddComponent<MeshComponent>(&entity);
        componentManager.AddComponent<PositionComponent>(&entity);
        componentManager.AddComponent<MovementComponent>(&entity);
        componentManager.AddComponent<CollisionComponent>(&entity);
        componentManager.AddComponent<HealthComponent>(&entity);
        componentManager.AddComponent<CombatComponent>(&entity);
        
        meshSystem.CreateCubeMesh(&entity, Color::Red);
        componentManager.GetComponentHandler<MovementComponent>()->GetComponent(&entity).Speed = 3.f;
        componentManager.GetComponentHandler<CombatComponent>()->GetComponent(&entity).damage = 1;
        componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entity).Health = 5;
        componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entity).Position =
                        glm::vec3(rand()%18-9, rand()%10-5, -10.f);
    }
    //Adding components to the pickup entities
    for(Entity entity: pickups)
    {
        componentManager.AddComponent<MeshComponent>(&entity);
        componentManager.AddComponent<PositionComponent>(&entity);
        componentManager.AddComponent<CollisionComponent>(&entity);

        meshSystem.CreateCubeMesh(&entity, Color::Green);
        componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entity).Position =
                glm::vec3(rand()%18-9, rand()%10-4, -10.f);
    }

    componentManager.AddComponent<MeshComponent>(&entities[entities.size()-1]);
    componentManager.AddComponent<PositionComponent>(&entities[entities.size()-1]);
    meshSystem.CreateFloorMesh(&entities[entities.size()-1]);
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entities[entities.size()-1]).Position =
            glm::vec3(0.f,0.f,0.f);
    
}

void Engine::setup()
{
    Window = Window::init();
    MainCamera.init();
    glLineWidth(5.f);
    Create();
}

void Engine::Draw()
{
    for(Entity entity : entities)
    {
        meshSystem.DrawMesh(&entity);
    }
}

void Engine::update()
{
    if(static_cast<ComponentHandler<HealthComponent>*>(componentManager.Components[typeid(HealthComponent)])->GetComponent(&entities[0]).Health <= 0)
    {
        std::cout << "Player has died" << std::endl;
        glfwSetWindowShouldClose(Window, true);
    }
    collisionSystem.UpdatePosition(&entities[0]);
    for(Entity entity : enemies)
    {
        collisionSystem.UpdatePosition(&entity);
        if(collisionSystem.CheckCollision(&entities[0], &entity))
        {
            combatSystem.Attack(&entity, &entities[0]);
            static_cast<ComponentHandler<MovementComponent>*>(componentManager.Components[typeid(MovementComponent)])->
            GetComponent(&entity).Movement = glm::vec3(0.f);
        }
        else
        {
            movementSystem.FindDirection(&entity, &entities[0]);
            movementSystem.MoveEntity(&entity);
        }
        combatSystem.DelayTimer(&entity);
    }
   for (Entity entity : pickups)
   {
       collisionSystem.UpdatePosition(&entity);
       if(collisionSystem.CheckCollision(&entities[0], &entity))
       {
           componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entities[0]).Health += 1;
           componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entity).Position =
                   glm::vec3(rand()%18-9, rand()%10-5, -10.f);
       }
   }
    Draw();
    MainCamera.OrbitCamera();
}

void Engine::run()
{
    setup();
    float FirstFrame = 0.0f;
    float secondcounter = 0.f;
    glm::vec3 color(Color::Cyan);
    while(!glfwWindowShouldClose(Window))
    {
        const auto CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - FirstFrame;
        FirstFrame = CurrentFrame;
        secondcounter += DeltaTime;
        if(secondcounter >= 0.2f)
        {
            secondcounter = 0.f;
            std::cout << "Player Health: " << componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entities[0]).Health << std::endl;
        }
        
        glClearColor(color.x, color.y, color.z, 1.f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Shader::Program);
		  
        update();
		  
        glUniformMatrix4fv(MainCamera.projectionLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getProjection(Window::Width, Window::Height)));
        glUniformMatrix4fv(MainCamera.viewLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getView()));

        KeyBoardInput::processInput(Window, &entities[0], &componentManager);
    
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwTerminate();
}

