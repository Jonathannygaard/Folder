#pragma once
#include "Components/Component.h"
#include "System/System.h"
#include "Window/Camera/Camera.h"

struct GLFWwindow;

class Engine
{
public:
  GLFWwindow *Window;
  static inline Camera MainCamera;
  static inline float DeltaTime;
    
  int AmountOfSpheres = 1;
    
  int TerrainResolution = 6;
  int xLength;

  ComponentManager componentManager;
  MeshSystem meshSystem;
  MovementSystem movementSystem;
  CollisionSystem collisionSystem;
  CombatSystem combatSystem;

  void Create();
  void setup();
  void update();
  void run();
  void Draw();

  std::vector<Entity> entities;  
  std::vector<Entity> spheres;
  
  Engine() : meshSystem(componentManager), movementSystem(componentManager),
                collisionSystem(componentManager), combatSystem(componentManager) {}
};
