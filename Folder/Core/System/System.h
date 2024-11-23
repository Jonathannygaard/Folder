#pragma once
#include "../Components/Component.h"
#include "../Entity/Entity.h"

class System{};

class MovementSystem : public System
{
public:
    ComponentManager& componentmanager;
    MovementSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void MoveEntity(Entity* entity);
    void FindDirection(Entity* entity, Entity* target);
    void Gravity(Entity* entity);
};

class MeshSystem : public System
{
public:
    ComponentManager& componentmanager;
    MeshSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void DrawMesh(Entity* entity);
    void BindBuffers(Entity* entity);
    int SortPoints(std::vector<Vertex> points, glm::vec3 min, glm::vec3 max, Entity* entity, int resolution);
    int LoadPointCloud(const std::string& filename, Entity* entity, int resolution);
    void CreateCubeMesh(Entity* entity, glm::vec3 color);
    int CreateFloorMesh(Entity* entity, int resolution);
    void CreateSphereMesh(Entity* entity, int Sectors, int Stacks, float radius, glm::vec3 color);
};

class CollisionSystem : public System
{
public:
    ComponentManager& componentmanager;
    CollisionSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    bool CheckCollision(Entity* entity1, Entity* entity2);
    void UpdatePosition(Entity* entity);
    bool BarycentricCoordinates(Entity* terrain, Entity* entity, int resolution, int xLength);
};

class CombatSystem : public System
{
public:
    ComponentManager& componentmanager;
    CombatSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void Attack(Entity* entity1, Entity* entity2);
    void TakeDamage(Entity* entity, int damage);
    void DelayTimer(Entity* entity);
};

