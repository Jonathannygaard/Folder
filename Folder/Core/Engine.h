 #pragma once
#include "Components/Component.h"
#include "Lua/Lua.h"
#include "ParticleSystem/ParticleSystem.h"
#include "System/System.h"
#include "Window/Camera/Camera.h"

struct GLFWwindow;

class Engine
{
    inline static Engine* instance = nullptr;
    
public:
  GLFWwindow *Window;
  static inline Camera MainCamera;
  static inline float DeltaTime;
  Lua lua;
    
  int AmountOfSpheres = 10;
    
  int TerrainResolution = 6;
  int xLength;

  double totaltime = 0;

  float tracktimer = 0.f;
  float trackinterval = 0.1f;

  float Particletimer = 0;
  float Particleinterval = 0.05f;
    
  static inline bool isPointcloud = false;
    
  static inline bool isWireframe = false;

  ComponentManager componentManager;
  MeshSystem meshSystem;
  MovementSystem movementSystem;
  CollisionSystem collisionSystem;
  CombatSystem combatSystem;
  TrackingSystem trackingsystem;

  ParticleSystem particleSystem;

    static Engine& get();
    
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

  void Create();
  void CreateSphere(glm::vec3 position);
  void setup();
  void update();
  void run();
  void Draw();

  std::vector<Entity> entities;  
  std::vector<Entity> spheres;
  
  Engine() : meshSystem(componentManager), movementSystem(componentManager),
                collisionSystem(componentManager), combatSystem(componentManager), trackingsystem(componentManager){}
};
