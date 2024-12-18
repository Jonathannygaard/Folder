﻿#pragma once
#include <map>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>
#include "../Entity/Entity.h"

class Component{};
class ComponentH{};

struct Vertex
{
    glm::vec3 Position = glm::vec3(0,0,0);
    glm::vec3 Color = glm::vec3(0,0,0);
    glm::vec3 Normal = glm::vec3(0,0,0);
    float Friction = 0.5f;
    float Bounce = 0.6f;
    Vertex() = default;
    Vertex(glm::vec3 Pos, glm::vec3 rgb) : Position(Pos), Color(rgb){}
};

struct Triangles
{
    unsigned int a, b, c;
    Triangles(unsigned int a, unsigned int b, unsigned int c) : a(a), b(b), c(c){}
};

template<typename T>
class ComponentHandler : public ComponentH
{
public:
    std::vector<T> Components;
    std::map<int, int> EntityToIndex;

    void AddComponent(Entity* entity)
    {
        Components.emplace_back();
        EntityToIndex[entity->ID] = Components.size() - 1;
    }

    T& GetComponent(Entity* entity) 
    {
        if(EntityToIndex.contains(entity->ID))
        {
            return Components[EntityToIndex[entity->ID]];
        }
        throw std::runtime_error("Entity does not have this component");
    }

    bool HasComponent(Entity* entity)
    {
        if (entity == nullptr)
        {
            return false;
        }
        return EntityToIndex.contains(entity->ID);
    }
};

class ComponentManager
{
public:
    std::unordered_map<std::type_index, ComponentH*> Components;
    template<typename T>
    void AddComponent(Entity* entity)
    {
        if(!Components.contains(typeid(T)))
        {
            Components[typeid(T)] = new ComponentHandler<T>();
        }
        static_cast<ComponentHandler<T>*>(Components[typeid(T)]) -> AddComponent(entity);
    }
    template<typename T>
    ComponentHandler<T>* GetComponentHandler()
    {
        ComponentHandler<T> handler;
        return static_cast<ComponentHandler<T>*>(Components[typeid(T)]);
    }
};

class PositionComponent : public Component
{
public:
    glm::vec3 Position;
};

class TrackingComponent : public Component
{
public:
    std::vector<Vertex> SplinePoints;
    std::vector<glm::vec3> controlpoints;
    std::vector<int> knots;

    unsigned int VBO;
    unsigned int VAO;
};

class MovementComponent : public Component
{
public:
    glm::vec3 Movement;
};

class HealthComponent : public Component
{
public:
    int Health;

};

class MeshComponent : public Component
{
public:
    unsigned int VBO;
    unsigned int EBO;
    std::vector<Vertex> Vertices;
    std::vector<Triangles> Indices;

    unsigned int VAO;
};

class MassComponent: public Component
{
public:
    float Mass;
};

class CollisionComponent : public Component
{
public:
    glm::vec3 min;
    glm::vec3 max;
    float Radius;
};

class CombatComponent : public Component
{
public:
    int damage;
    float delay;
};
