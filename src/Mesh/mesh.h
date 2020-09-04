#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

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

Mesh GenerateMesh(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);
MeshIndexed GenerateMeshIndexed(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

Mesh GenerateCube();
Mesh GenerateAxes();