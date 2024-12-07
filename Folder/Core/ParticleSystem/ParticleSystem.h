#pragma once
#include <vector>
#include "../Shader/Shader.h"
#include "../Engine.h"
#include "../Color.h"

class ParticleSystem
{
public:
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Velocities;
    std::vector<glm::vec3> Colors;
    std::vector<float> Lifetimes;

    std::vector<Vertex> Vertices;
    std::vector<Triangles> Indices;

    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;

    int MaxParticles = 1000;
    int NumParticles = 0;

    ParticleSystem();
    void Emit(glm::vec3 position);
    void CreateParticle();
    void BindBuffers();
    void Update();
    void DrawParticles();
};