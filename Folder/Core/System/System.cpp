#include "System.h"

#include <algorithm>
#include <dinput.h>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Components/Component.h"
#include "../Shader/Shader.h"
#include "../Engine.h"

void MovementSystem::MoveEntity(Entity* entity)
{
    static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position +=
        static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement *
            static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Speed
    * Engine::DeltaTime;
}

void MovementSystem::FindDirection(Entity* entity, Entity* target)
{
    static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement =
        glm::normalize(static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(target).Position -
            static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position);
}

void MeshSystem::DrawMesh(Entity* entity)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position);
    glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    MeshComponent& mesh = static_cast<ComponentHandler<MeshComponent>*>(componentmanager.Components[typeid(MeshComponent)])->GetComponent(entity);
    glBindVertexArray(mesh.VAO);
    
    if(componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Indices.empty())
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh.Vertices.size());
        glBindVertexArray(0);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mesh.Indices.size() * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void MeshSystem::BindBuffers(Entity* entity)
{
    if(componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Indices.empty())
    {
        ComponentHandler<MeshComponent>* componenthandler = componentmanager.GetComponentHandler<MeshComponent>();
        MeshComponent& mesh = componenthandler->GetComponent(entity);
        // VAO
        glGenBuffers(1, &mesh.VBO);

        // VAO
        glGenVertexArrays(1, &mesh.VAO);
        glBindVertexArray(mesh.VAO);

        // VBO
        glGenBuffers(1, &mesh.EBO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), mesh.Vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    ComponentHandler<MeshComponent>* componenthandler = componentmanager.GetComponentHandler<MeshComponent>();
    MeshComponent& mesh = componenthandler->GetComponent(entity);
    // VAO
    glGenBuffers(1, &mesh.VBO);

    // VAO
    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);

    // VBO
    glGenBuffers(1, &mesh.EBO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), mesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(Triangles), mesh.Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void MeshSystem::SortPoints(std::vector<Vertex> points, glm::vec3 min, glm::vec3 max, Entity* entity)
{
    std::vector<Vertex> Temp;
    std::vector<Triangles> Indices;
    int quality = 6;
    int maxX = static_cast<int>(max.x * 100.f) >> quality;
    int maxZ = static_cast<int>(max.z * 100.f) >> quality;
    int xLength = maxX + 1;
    int zLength = maxZ + 1;
    
    std::vector<int> PointCount (xLength * zLength, 0);

    Temp.resize(xLength * zLength);

    int index = 0;
    for(int x = 0; x < xLength; x++)
    {
        for(int z = 0; z < zLength; z++)
        {
            index = (z * xLength) + x;
            Temp[index].Position.x = (x << quality);
            Temp[index].Position.x *= 0.01f;
            Temp[index].Position.y = 0;
            Temp[index].Position.z = (z << quality);
            Temp[index].Position.z *= 0.01f;
        }
    }
    for (Vertex point: points)
    {
        int xPos = static_cast<int>(point.Position.x * 100.f) >> quality;
        int zPos = static_cast<int>(point.Position.z * 100.f) >> quality;
        index = (zPos * xLength) + xPos;
        
        PointCount[index]++;
        Temp[index].Position.y += point.Position.y;
        Temp[index].Color += point.Color;
    }
    for (int i = 0; i < Temp.size(); i++)
    {
        if (PointCount[i] > 0)
        {
            Temp[i].Position.y /= PointCount[i];
            Temp[i].Color /= PointCount[i];
        }
        else
        {
            if (i-1 < 0)
            {
                Temp[i].Position.y = 0;
                continue;
            }
            Temp[i].Position.y = Temp[i-1].Position.y;
            Temp[i].Color = Temp[i-1].Color;
        }
    }
    for (int i = 0; i < Temp.size(); i++)
    {
        if ((i + 1) % xLength == 0)
        {
            continue;
        }
        if (i + xLength >= Temp.size())
        {
            continue;
        }
        Indices.emplace_back(i, i + xLength, i + xLength + 1);
        Indices.emplace_back(i, i + xLength + 1, i + 1);
    }
    componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Vertices = Temp;
    componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Indices = Indices;
    BindBuffers(entity);
}

void MeshSystem::LoadPointCloud(const std::string& filename, Entity* entity)
{
    std::vector<Vertex> points;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << std::endl;
        return;
    }

    glm::vec3 minPoint(FLT_MAX);
    glm::vec3 maxPoint(-FLT_MAX);
    bool skipLine = true;

    while (std::getline(file, line))
    {
        if (skipLine)
        {
            skipLine = !skipLine;
            continue;       
        }
     
            //Position and Color
            float x, y, z, r, g, b;

            //Useless data
            float nx, ny, nz, a;
        
            //Supports both space and tab seperated values
            if (sscanf_s(line.c_str(), "%f %f %f %f %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b, &nx, &ny, &nz, &a) == 10 || sscanf_s(line.c_str(), "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f", &x, &y, &z, &r, &g, &b, &nx, &ny, &nz, &a) == 10)
            {
                float xf = x;
                float yf = z;
                float zf = y;
                float rf = r;
                float gf = g;
                float bf = b;
                Vertex point(glm::vec3(xf, yf, zf), glm::vec3(rf, gf, bf));

                // Update min/max bounds
                minPoint = glm::min(minPoint, point.Position);
                maxPoint = glm::max(maxPoint, point.Position);

                points.push_back(point);
                skipLine = !skipLine;
            }
    }
    maxPoint -= minPoint;
    file.close();

    // Translate points to move the minimum point to the origin
    for (auto &point : points)
    {
        point.Position -= minPoint;
    }
    SortPoints(points, minPoint,maxPoint,entity);
}

void MeshSystem::CreateCubeMesh(Entity* entity, glm::vec3 color)
{
    ComponentHandler<MeshComponent>* componenthandler = componentmanager.GetComponentHandler<MeshComponent>();
    MeshComponent& mesh = componenthandler->GetComponent(entity);
    
    Vertex v0{glm::vec3(0.f, 0.f, 0.f), color}; /* Front-Bot-left */
    Vertex v1{ glm::vec3(1.f, 0.f, 0.f), color}; /* Front-Bot-right */
    Vertex v2{ glm::vec3(1.f, 1.f, 0.f), color}; /* Front-Top-right */
    Vertex v3{ glm::vec3(0.f, 1.f, 0.f), color}; /* Front-Top-left */
    Vertex v4{ glm::vec3(0.f, 0.f, -1.f),color}; /* Back-Bot-left */
    Vertex v5{ glm::vec3(1.f, 0.f, -1.f),color}; /* Back-Bot-right */
    Vertex v6{ glm::vec3(1.f, 1.f, -1.f),color}; /* Back-Top-right */
    Vertex v7{ glm::vec3(0.f, 1.f, -1.f),color}; /* Back-Top-left */

    mesh.Vertices.emplace_back(v0);
    mesh.Vertices.emplace_back(v1);
    mesh.Vertices.emplace_back(v2);
    mesh.Vertices.emplace_back(v3);
    mesh.Vertices.emplace_back(v4);
    mesh.Vertices.emplace_back(v5);
    mesh.Vertices.emplace_back(v6);
    mesh.Vertices.emplace_back(v7);

    /* Front */
    mesh.Indices.emplace_back(0,1,2);
    mesh.Indices.emplace_back(2,3,0);

    /* Right */
    mesh.Indices.emplace_back(1,5,6);
    mesh.Indices.emplace_back(6,2,1);

    /* Left */			   
    mesh.Indices.emplace_back(0,3,7);
    mesh.Indices.emplace_back(7,4,0);
						   
    /* Back */
    mesh.Indices.emplace_back(4,7,6);
    mesh.Indices.emplace_back(6,5,4);

    /* Top */
    mesh.Indices.emplace_back(3,2,6);
    mesh.Indices.emplace_back(6,7,3);

    /* Bottom */
    mesh.Indices.emplace_back(0,4,5);
    mesh.Indices.emplace_back(5,1,0);

    BindBuffers(entity);
}

void MeshSystem::CreateFloorMesh(Entity* entity)
{
    LoadPointCloud("map.txt", entity);
}

bool CollisionSystem::CheckCollision(Entity* entity1, Entity* entity2)
{
    if(entity1 == nullptr || entity2 == nullptr)
    {
        return false;
    }
    if(entity1 == entity2)
    {
        return false;
    }
    glm::vec3 min1 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity1).min;
    glm::vec3 max1 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity1).max;
    glm::vec3 min2 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity2).min;
    glm::vec3 max2 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity2).max;
    
    if(min2.x < max1.x && max2.x > min1.x &&
        min2.y < max1.y && max2.y > min1.y &&
        max1.z < min2.z && min1.z > max2.z)
    {
        return true;
    }
    return false;
}

void CollisionSystem::UpdatePosition(Entity* entity)
{
    static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity).min =
        static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position;

    static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity).max =
        static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position
            + glm::vec3 (1.f, 1.f, -1.f);
}

void CombatSystem::Attack(Entity* entity1, Entity* entity2)
{
    if(static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity1).delay <= 0)
    {
        TakeDamage(entity2, static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity1).damage);
        static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity1).delay = 0.5f;
    }
}

void CombatSystem::TakeDamage(Entity* entity, int damage)
{
    static_cast<ComponentHandler<HealthComponent>*>(componentmanager.Components[typeid(HealthComponent)])->GetComponent(entity).Health -= damage;
}

void CombatSystem::DelayTimer(Entity* entity)
{
    if(static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity).delay >= 0)
    {
        static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity).delay -= Engine::DeltaTime;
    }
}