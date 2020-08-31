#pragma once
#include <glm/vec3.hpp>

struct Mesh
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int numVertices;
};

struct MeshIndexed
{
    unsigned int VAO;
    unsigned int VBO[3];
    unsigned int EBO;
    unsigned int numVertices;

    unsigned int diffuseIndex;
};

struct Entity
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

MeshIndexed GenerateMeshIndexed(float* vertices, size_t numVertices, unsigned int* indices, size_t numIndices, float* uvs, size_t numUVs, float* normals, size_t numNormals);
Mesh GenerateCube();
Mesh GenerateAxes();