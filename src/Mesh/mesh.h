#pragma once
#include "../Math/vec3.h"
#include "../Math/vec2.h"
#include <vector>

// TEMP
#include <glm/vec3.hpp>

struct Mesh
{
    char name[128];
    unsigned int VAO;
    unsigned int VBO[5];
    unsigned int numVertices;
};

struct MeshIndexed
{
    char name[128];
    unsigned int VAO;
    unsigned int VBO[5];
    unsigned int EBO;
    unsigned int numVertices;
};

struct Entity
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

void Draw(Mesh& mesh);
void DrawLines(Mesh& mesh);
void Draw(MeshIndexed& mesh);

Mesh GenerateMesh(std::vector<Vec3f>& vertices, std::vector<Vec2f>& uvs, std::vector<Vec3f>& normals, std::vector<Vec3f>& tangents, std::vector<Vec3f>& bitangents);
MeshIndexed GenerateMeshIndexed(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices, std::vector<Vec2f>& uvs, std::vector<Vec3f>& normals, std::vector<Vec3f>& tangents, std::vector<Vec3f>& bitangents);

Mesh GenerateCube();
Mesh GenerateAxes();