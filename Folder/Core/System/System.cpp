#include "System.h"
#include <algorithm>
#include <dinput.h>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Color.h"
#include "../Components/Component.h"
#include "../Shader/Shader.h"
#include "../Engine.h"

void MovementSystem::MoveEntity(Entity* entity)
{
    static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position +=
        static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement * Engine::DeltaTime;
}

void MovementSystem::FindDirection(Entity* entity, Entity* target)
{
    static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement =
        glm::normalize(static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(target).Position -
            static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position);
}

void MovementSystem::Gravity(Entity* entity)
{
    float force = componentmanager.GetComponentHandler<MassComponent>()->GetComponent(entity).Mass * -0.1f; 
    componentmanager.GetComponentHandler<MovementComponent>()->GetComponent(entity).Movement +=
            glm::vec3(0,force,0);
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
        glDrawArrays(GL_POINTS, 0, mesh.Vertices.size());
        glBindVertexArray(0);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mesh.Indices.size() * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    if (componentmanager.GetComponentHandler<TrackingComponent>()->HasComponent(entity))
    {
        glLineWidth(10.f);
        TrackingComponent& tracking = componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity);
        glBindVertexArray(tracking.VAO);
        glDrawArrays(GL_LINE_STRIP, 0, tracking.SplinePoints.size());
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
    else
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
    if (componentmanager.GetComponentHandler<TrackingComponent>()->HasComponent(entity))
    {
        ComponentHandler<TrackingComponent>* componenthandler = componentmanager.GetComponentHandler<TrackingComponent>();
        TrackingComponent& tracking = componenthandler->GetComponent(entity);
        glGenBuffers(1, &tracking.VBO);
        glGenVertexArrays(1, &tracking.VAO);
        glBindVertexArray(tracking.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, tracking.VBO);
        glBufferData(GL_ARRAY_BUFFER, tracking.SplinePoints.size() * sizeof(Vertex), tracking.SplinePoints.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void MeshSystem::UpdateBuffers(Entity* entity)
{
    TrackingComponent& tracking = componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity);

    // Bind the VAO
    glBindVertexArray(tracking.VAO);

    glBufferData(GL_ARRAY_BUFFER, tracking.SplinePoints.size() * sizeof(Vertex), tracking.SplinePoints.data(), GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    // Update vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, tracking.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tracking.SplinePoints.size() * sizeof(Vertex), tracking.SplinePoints.data());

    // Unbind the VAO
    glBindVertexArray(0);
}


int MeshSystem::SortPoints(std::vector<Vertex> points, glm::vec3 min, glm::vec3 max, Entity* entity, int resolution)
{
    std::vector<Vertex> Temp;
    std::vector<Triangles> Indices;
    int maxX = static_cast<int>(max.x * 100.f) >> resolution;
    int maxZ = static_cast<int>(max.z * 100.f) >> resolution;
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
            Temp[index].Position.x = (x << resolution);
            Temp[index].Position.x *= 0.01f;
            Temp[index].Position.y = 0;
            Temp[index].Position.z = (z << resolution);
            Temp[index].Position.z *= 0.01f;
            Temp[index].Color = glm::vec3(0);
        }
    }
    for (Vertex point: points)
    {
        int xPos = static_cast<int>(point.Position.x * 100.f) >> resolution;
        int zPos = static_cast<int>(point.Position.z * 100.f) >> resolution;
        index = (zPos * xLength) + xPos;
        
        PointCount[index]++;
        Temp[index].Position.y += point.Position.y;
        Temp[index].Color += point.Color;
    }
    float minHeight;
    for (int i = 0; i < Temp.size(); i++)
    {
        if (PointCount[i] > 0)
        {
            Temp[i].Position.y /= PointCount[i];
            Temp[i].Color /= PointCount[i];
        }
        else
        {
            Temp[i].Color = glm::vec3(0);
            if (i-1 >= 0 && i % xLength != 0)
            {
                Temp[i].Position.y += Temp[i-1].Position.y;
                Temp[i].Color += Temp[i - 1].Color;  
                PointCount[i]++;
            }
            if (i + 1 < Temp.size() && (i + 1) % xLength != 0)
            {
                if (PointCount[i+1]>0)
                {
                    Temp[i].Position.y += Temp[i + 1].Position.y/PointCount[i + 1];
                    Temp[i].Color += Temp[i + 1].Color/glm::vec3(PointCount[i + 1]);
                    PointCount[i]++;
                }
            }
            if (i + xLength < Temp.size())
            {
                if (PointCount[i + xLength] > 0)
                {
                    Temp[i].Position.y += Temp[i + xLength].Position.y/PointCount[i + xLength];
                    Temp[i].Color += Temp[i + xLength].Color/glm::vec3(PointCount[i + xLength]);
                    PointCount[i]++;
                }
            }
            if (i - xLength >= 0)
            {
                Temp[i].Position.y += Temp[i - xLength].Position.y;
                Temp[i].Color += Temp[i - xLength].Color;
                PointCount[i]++;
            }
            Temp[i].Position.y /= PointCount[i];
            Temp[i].Color /= PointCount[i];
        }
        if (i == 0)
        {
            minHeight = Temp[i].Position.y;
        }
    }
    for (int i = 0; i < Temp.size(); i++)
    {
        Temp[i].Position.y -= minHeight;
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
    for (auto Triangles: Indices)
    {
        glm::vec3 Normal = glm::normalize(glm::cross(Temp[Triangles.a].Position - Temp[Triangles.c].Position,
            Temp[Triangles.b].Position - Temp[Triangles.c].Position));
        Temp[Triangles.a].Normal += Normal;
        Temp[Triangles.b].Normal += Normal;
        Temp[Triangles.c].Normal += Normal;
    }
    
    componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Vertices = Temp;
    componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Indices = Indices;
    BindBuffers(entity);
    return xLength;
}

int MeshSystem::LoadPointCloud(const std::string& filename, Entity* entity, int resolution)
{
    std::vector<Vertex> points;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << std::endl;
        return 0;
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
    return SortPoints(points, minPoint,maxPoint,entity, resolution);
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

int MeshSystem::CreateFloorMesh(Entity* entity, int resolution)
{
    return LoadPointCloud("map.txt", entity, resolution);
}


void MeshSystem::CreateSphereMesh(Entity* entity, int Sectors, int Stacks, float radius, glm::vec3 color)
{
    float x, y, z, xy;
    float SectorStep = 2 * glm::pi<float>() / Sectors;
    float StackStep = glm::pi<float>() / Stacks;
    float SectorAngle, StackAngle;

    for(int i = 0; i <= Stacks; i++)
    {
        StackAngle = glm::pi<float>() / 2 - i * StackStep;
        float xy = radius * cos(StackAngle);
        float z = radius * sin(StackAngle);
		
        for(int j = 0; j <= Sectors; j++)
        {
            SectorAngle = j * SectorStep;

            float x = xy * cos(SectorAngle);
            float y = xy * sin(SectorAngle);
            glm::vec3 Temp(x,y,z);
            if(i == 0 && j != 0)
            {
                //continue;
            }
				
            componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Vertices.emplace_back(Temp, color);
        }
        for(int i = 0; i < Stacks; i++)
        {
            int k1 = i * (Sectors + 1);
            int k2 = k1 + Sectors + 1;
	
            for(int j = 0; j < Sectors; j++, k1++, k2++)
            {
                if(i != 0)
                {
                    componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Indices.emplace_back(k1, k2, k1 + 1);
                }
                if(i != (Stacks - 1))
                {
                    componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(entity).Indices.emplace_back(k1 + 1, k2, k2 + 1);
                }
            }
        }
        BindBuffers(entity);
    }
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

bool CollisionSystem::CheckSphereCollision(Entity* entity1, Entity* entity2)
{
    CollisionComponent& collision = componentmanager.GetComponentHandler<CollisionComponent>()->GetComponent(entity1);
    CollisionComponent& other = componentmanager.GetComponentHandler<CollisionComponent>()->GetComponent(entity2);
    
    if(glm::distance(collision.min, other.min) < collision.Radius + other.Radius)
    {
        ResolveSphereCollision(entity1, entity2);
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

void CollisionSystem::ResolveGroundCollision(Entity* entity, float height, glm::vec3 Normal)
{
    // Get components
    PositionComponent& position = componentmanager.GetComponentHandler<PositionComponent>()->GetComponent(entity);
    MovementComponent& velocity = componentmanager.GetComponentHandler<MovementComponent>()->GetComponent(entity);
    
    // Normalize the surface normal
    Normal = glm::normalize(Normal);    
    
    // Calculate penetration depth
    float penetrationDepth = (height + 0.5f) - position.Position.y;

    // Resolution parameters
    const float BOUNCINESS = 0.6f;
    const float FRICTION = 0.95f;
    const float STOP_THRESHOLD = 0.01f;
    const float ROLLING_THRESHOLD = 0.5f;
    
    // Resolve position (move sphere out of ground)
    position.Position.y = height + 0.5f;
    
    // Calculate reflection vector
    float dotProduct = glm::dot(velocity.Movement, Normal);
    if (dotProduct < 0) 
    {
        // Basic reflection formula: v = v - 2(v·n)n
        glm::vec3 reflection = velocity.Movement - 2.0f * dotProduct * Normal;

        // Split velocity into normal and tangential components
        glm::vec3 normalComponent = dotProduct * Normal;
        glm::vec3 tangentialComponent = velocity.Movement - normalComponent;
        
        // Apply bounciness to normal component
        normalComponent = -normalComponent * BOUNCINESS;
        
        // Apply friction to tangential component
        tangentialComponent *= FRICTION;
        
        // Combine components
        velocity.Movement = normalComponent + tangentialComponent;
        
        // Handle rolling
        if (glm::length(velocity.Movement) < ROLLING_THRESHOLD)
        {
            // Calculate roll direction based on slope
            glm::vec3 gravity(0.0f, -1.f, 0.0f);
            glm::vec3 rollDirection = glm::cross(Normal, glm::cross(gravity, Normal));
            
            if (glm::length(rollDirection) > 0.0f)
            {
                rollDirection = glm::normalize(rollDirection);
                velocity.Movement += rollDirection * 0.1f;
            }
        }
        
        // Stop if moving very slowly
        if (glm::length(velocity.Movement) < STOP_THRESHOLD)
        {
            velocity.Movement = glm::vec3(0.0f);
        }
    }
}

void CollisionSystem::ResolveSphereCollision(Entity* entity1, Entity* entity2)
{
    CollisionComponent& entity = componentmanager.GetComponentHandler<CollisionComponent>()->GetComponent(entity1);
    CollisionComponent& other = componentmanager.GetComponentHandler<CollisionComponent>()->GetComponent(entity2);

    MovementComponent& moveEntity = componentmanager.GetComponentHandler<MovementComponent>()->GetComponent(entity1);
    MovementComponent& moveOther = componentmanager.GetComponentHandler<MovementComponent>()->GetComponent(entity2);

    MassComponent& massEntity = componentmanager.GetComponentHandler<MassComponent>()->GetComponent(entity1);
    MassComponent& massOther = componentmanager.GetComponentHandler<MassComponent>()->GetComponent(entity1);
    
    glm::vec3 normal = glm::normalize(entity.min - other.min);
    glm::vec3 relativeVelocity = moveEntity.Movement - moveOther.Movement;
    float VelosityAlongNormal = glm::dot(relativeVelocity, normal);

    if(VelosityAlongNormal > 0)
        return;

    float restitution = 1.f; //perfect elasticity
    float impulse = (-(1 + restitution) * VelosityAlongNormal) / (1 / massEntity.Mass + 1 / massOther.Mass);

    glm::vec3 impulseVector = impulse * normal;
    moveEntity.Movement += impulseVector / massEntity.Mass;
    moveOther.Movement -= impulseVector / massOther.Mass;
}

bool CollisionSystem::BarycentricCoordinates(Entity* terrain, Entity* entity, int resolution, int xLength)
{
    glm::vec3 p1, p2, p3, player, temp, terrainPos;
    
    player = componentmanager.GetComponentHandler<PositionComponent>()->GetComponent(entity).Position;
    MeshComponent& terrainMesh = componentmanager.GetComponentHandler<MeshComponent>()->GetComponent(terrain);
    
    int xPos = static_cast<int>(player.x * 100) >> resolution;
    int zPos = static_cast<int>(player.z * 100) >> resolution;

    int index = (zPos * xLength) + xPos;
    
    // Early exit if index is out of bounds
    if (index < 0 || index + xLength + 1 >= terrainMesh.Vertices.size())
    {
        return false;
    }
    
    for(int i = 0; i < 2; i++)
    {
        if(i==0)
        {
            p1 = terrainMesh.Vertices[index].Position;
            p2 = terrainMesh.Vertices[index + xLength].Position;
            p3 = terrainMesh.Vertices[index + xLength + 1].Position;
        }
        else
        {
            p1 = terrainMesh.Vertices[index].Position;
            p2 = terrainMesh.Vertices[index + xLength +1].Position;
            p3 = terrainMesh.Vertices[index + 1].Position;
        }

        // Calculate barycentric coordinates
        glm::vec2 v0 = glm::vec2(p2.x - p1.x, p2.z - p1.z);
        glm::vec2 v1 = glm::vec2(p3.x - p1.x, p3.z - p1.z);
        glm::vec2 v2 = glm::vec2(player.x - p1.x, player.z - p1.z);

        // Compute dot products
        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);

        // Compute barycentric coordinates
        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float v = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float w = (dot00 * dot12 - dot01 * dot02) * invDenom;
        float u = 1.0f - v - w;

        // Check if point is in triangle
        if (u >= 0.0f && v >= 0.0f && w >= 0.0f)
        {
            // Interpolate height
            float height = u * p1.y + v * p2.y + w * p3.y;
            if (componentmanager.GetComponentHandler<PositionComponent>()->GetComponent(entity).Position.y <= height + 0.5f)
            {
                glm::vec3 Normal;
                if (i == 0)
                {
                    glm::vec3 edge1 = p2 - p1;
                    glm::vec3 edge2 = p3 - p1;
                    Normal = glm::normalize(glm::cross(edge1, edge2));
                }
                else
                {
                    glm::vec3 edge1 = p2 - p1;
                    glm::vec3 edge2 = p3 - p1;
                    Normal = glm::normalize(glm::cross(edge1, edge2));
                }
                ResolveGroundCollision(entity, height, Normal);
                return true;
            }
        }
    }
    return false;
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

float TrackingSystem::BasisFunction(Entity* entity, int i, int k, float t)
{
    TrackingComponent& tracking_component = componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity);
    if (k == 1)
    {
        if (t >= tracking_component.knots[i] && t < tracking_component.knots[i + 1]) return 1.0f;
        return 0.0f;
    }
        
    float d1 = tracking_component.knots[i + k - 1] - tracking_component.knots[i];
    float d2 = tracking_component.knots[i + k] - tracking_component.knots[i + 1];
        
    float c1 = (d1 != 0) ? (t - tracking_component.knots[i]) / d1 * BasisFunction(entity, i, k - 1, t) : 0;
    float c2 = (d2 != 0) ? (tracking_component.knots[i + k] - t) / d2 * BasisFunction(entity, i + 1, k - 1, t) : 0;
        
    return c1 + c2;
}

void TrackingSystem::GenerateKnots(Entity* entity, int degree)
{
    TrackingComponent& tracking_component = componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity);
    int n = tracking_component.controlpoints.size() - 1;
    int m = n + degree + 1;

    tracking_component.knots.clear();
    
    // Generate uniform knot vector
    for (int i = 0; i < m; i++)
    {
        if (i <= degree)
        {
            tracking_component.knots.push_back(0);
        }
        else if (i >= m - degree - 1)
        {
            tracking_component.knots.push_back(m - 2 * (degree + 1) + 1);
        }
        else
        {
            tracking_component.knots.push_back(i - degree);
        }
    }
}

glm::vec3 TrackingSystem::Evaluate(Entity* entity, float t, int degree)
{
    TrackingComponent& tracking_component = componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity);
    // if (tracking_component.controlpoints.size() < 4) return glm::vec3(0.0f); // Need at least 4 points for cubic
    //
    // glm::vec3 point(0.0f);
    // int n = tracking_component.controlpoints.size() - 1;
    //
    // for (int i = 0; i <= n; i++)
    // {
    //     float basis = BasisFunction(entity, i, degree, t);
    //     point += tracking_component.controlpoints[i] * basis;
    // }
    //
    // return point;


    if (tracking_component.controlpoints.size() < 4) {
        // Need at least 4 points for a meaningful B-Spline
        return glm::vec3(0.0f);
    }
    
    // Ensure t is between 0 and 1
    t = glm::clamp(t, 0.0f, 1.0f);
    
    // Calculate the segment index
    int numSegments = tracking_component.controlpoints.size() - 3;
    int segmentIndex = static_cast<int>(t * numSegments);
    float localT = (t * numSegments) - segmentIndex;
    
    // Adjust indices to ensure we don't go out of bounds
    segmentIndex = glm::clamp(segmentIndex, 0, numSegments - 1);
    
    // Control points for current segment
    glm::vec3 p0 = tracking_component.controlpoints[segmentIndex];
    glm::vec3 p1 = tracking_component.controlpoints[segmentIndex + 1];
    glm::vec3 p2 = tracking_component.controlpoints[segmentIndex + 2];
    glm::vec3 p3 = tracking_component.controlpoints[segmentIndex + 3];
    
    // B-Spline basis functions
    float t2 = localT * localT;
    float t3 = t2 * localT;
    
    float b0 = (1.0f - 3.0f * localT + 3.0f * t2 - t3) / 6.0f;
    float b1 = (4.0f - 6.0f * t2 + 3.0f * t3) / 6.0f;
    float b2 = (1.0f + 3.0f * localT + 3.0f * t2 - 3.0f * t3) / 6.0f;
    float b3 = t3 / 6.0f;
    
    // Calculate the point on the curve
    return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}

void TrackingSystem::CreateBSpline(Entity* entity, int numPoints, glm::vec3 color, int degree, MeshSystem* mesh_system)
{
    // GenerateKnots(entity, degree);
    //
    // std::vector<Vertex> curvePoints;
    // curvePoints.reserve(numPoints);
    //     
    // float step = 1.0f / (numPoints - 1);
    // for (float t = 0.0f; t <= 1.0f; t += step)
    // {
    //     Vertex v = Vertex(Evaluate(entity, t, degree), color);
    //     curvePoints.push_back(v);
    // }
    // componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).SplinePoints = curvePoints;
    // mesh_system->UpdateBuffers(entity);


    
    std::vector<Vertex> trajectoryPoints;
        
    for (int i = 0; i < numPoints; ++i) {
        float t = static_cast<float>(i) / (numPoints - 1);
        trajectoryPoints.emplace_back(Evaluate(entity, t, degree), color);
    }
    
    componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).SplinePoints = trajectoryPoints;
    componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).controlpoints;
    componentmanager.GetComponentHandler<PositionComponent>()->GetComponent(entity).Position;
    mesh_system->UpdateBuffers(entity);
}

void TrackingSystem::TrackSphere(Entity* entity, MeshSystem* mesh_system)
{
    glm::vec3 Pos = componentmanager.GetComponentHandler<PositionComponent>()->GetComponent(entity).Position;
    glm::vec3 Color = Color::Yellow;
    int numPoints = 100;
    componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).controlpoints.push_back(Pos);

    if(componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).controlpoints.size() > numPoints)
    {
        componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).controlpoints.erase(componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).controlpoints.begin());
    }
    if (componentmanager.GetComponentHandler<TrackingComponent>()->GetComponent(entity).controlpoints.size() > 4)
    {
        CreateBSpline(entity, numPoints, Color, 2, mesh_system);
    }
}
