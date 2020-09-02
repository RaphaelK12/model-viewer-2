#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct Mesh
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int numVertices;
};


struct MeshIndexed
{
    unsigned int VAO;
    unsigned int VBO[5];
    unsigned int EBO;
    unsigned int numVertices;

    // TEMP
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    unsigned int diffuseIndex;
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

MeshIndexed GenerateMeshIndexed(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

Mesh GenerateCube();
Mesh GenerateAxes();