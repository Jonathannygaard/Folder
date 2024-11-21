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

  std::vector<Entity> pickups;  
  std::vector<Entity> enemies;  
  std::vector<Entity> entities;
  
  Engine() : meshSystem(componentManager), movementSystem(componentManager),
                collisionSystem(componentManager), combatSystem(componentManager) {}
};
