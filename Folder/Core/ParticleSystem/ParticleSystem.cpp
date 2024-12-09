#include "ParticleSystem.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Engine.h"
#include "../Components/Component.h"

ParticleSystem::ParticleSystem()
{
    MaxParticles = 1000000; //1 million particles
    NumParticles = 0;
}

//Emit new particle
void ParticleSystem::Emit(glm::vec3 position)
{
    if (NumParticles < MaxParticles)
    {
        Positions.push_back(position);
        Velocities.emplace_back(glm::vec3(0, -2, 0));
        Colors.emplace_back(Color::White);
        Lifetimes.emplace_back(20.f);
        NumParticles++;
    }
}

void ParticleSystem::CreateParticle()
{
    Vertex v0 = {glm::vec3(0.f, 0.f, 0.5f), Color::White};
    Vertex v1 = {glm::vec3(1.f, 0.f, 0.5f), Color::White};
    Vertex v2 = {glm::vec3(1.f, 1.f, 0.5f), Color::White};
    Vertex v3 = {glm::vec3(0.f, 1.f, 0.5f), Color::White};

    Vertex v4 = {glm::vec3(0.5f, 0.f, 0.f), Color::White};
    Vertex v5 = {glm::vec3(0.5f, 0.f, 1.f), Color::White};
    Vertex v6 = {glm::vec3(0.5f, 1.f, 1.f), Color::White};
    Vertex v7 = {glm::vec3(0.5f, 1.f, 0.f), Color::White};

    Vertex v8 = {glm::vec3(0.f, 0.5f, 0.f), Color::White};
    Vertex v9 = {glm::vec3(1.f, 0.5f, 0.f), Color::White};
    Vertex v10 = {glm::vec3(1.f, 0.5f, 1.f), Color::White};
    Vertex v11 = {glm::vec3(0.f, 0.5f, 1.f), Color::White};

    Vertices.push_back(v0);
    Vertices.push_back(v1);
    Vertices.push_back(v2);
    Vertices.push_back(v3);
    Vertices.push_back(v4);
    Vertices.push_back(v5);
    Vertices.push_back(v6);
    Vertices.push_back(v7);
    Vertices.push_back(v8);
    Vertices.push_back(v9);
    Vertices.push_back(v10);
    Vertices.push_back(v11);

    Indices.emplace_back(0,1,2);
    Indices.emplace_back(2,3,0);

    Indices.emplace_back(4,5,6);
    Indices.emplace_back(6,7,4);

    Indices.emplace_back(8,9,10);
    Indices.emplace_back(10,11,8);

    BindBuffers();
}

void ParticleSystem::BindBuffers()
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Triangles), Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::Update()
{
    for (int i = 0; i < NumParticles; i++)
    {
        Lifetimes[i] -= Engine::DeltaTime;
        Positions[i] += Velocities[i] * Engine::DeltaTime;

        if (Positions[i].y < 1)
        {
            Velocities[i] = glm::vec3(0, 0, 0);
            Positions[i].y = 1;
        }
        if (Lifetimes[i] <= 0)
        {
         DeleteParticle(i);   
        }
    }
}

void ParticleSystem::DrawParticles()
{
    for (int i = 0; i < NumParticles; i++)
    {
        //Draw particle
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[i]);
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, Indices.size() * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void ParticleSystem::DeleteParticle(int i)
{
    NumParticles--;
    Positions.erase(Positions.begin() + i);
    Velocities.erase(Velocities.begin() + i);
    Colors.erase(Colors.begin() + i);
    Lifetimes.erase(Lifetimes.begin() + i);
}

