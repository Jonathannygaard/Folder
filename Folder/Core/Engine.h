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
    
  int AmountOfSpheres = 10;
    
  int TerrainResolution = 6;
  int xLength;

  float tracktimer = 0.f;
  float trackinterval = 0.1f;

    static inline bool isPointcloud = false;
    
    static inline bool isWireframe = false;

  ComponentManager componentManager;
  MeshSystem meshSystem;
  MovementSystem movementSystem;
  CollisionSystem collisionSystem;
  CombatSystem combatSystem;
  TrackingSystem trackingsystem;

  void Create();
  void CreateSphere();
  void setup();
  void update();
  void run();
  void Draw();

  std::vector<Entity> entities;  
  std::vector<Entity> spheres;
  
  Engine() : meshSystem(componentManager), movementSystem(componentManager),
                collisionSystem(componentManager), combatSystem(componentManager), trackingsystem(componentManager) {}
};
