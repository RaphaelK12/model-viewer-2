#include "mesh.h"
#include <glad/glad.h>

MeshIndexed GenerateMeshIndexed(float* vertices, size_t numVertices, unsigned int* indices, size_t numIndices, float* uvs, size_t numUVs, float* normals, size_t numNormals)
{
    MeshIndexed result;
    result.numVertices = numIndices;

    glGenVertexArrays(1, &result.VAO);
    glBindVertexArray(result.VAO);
    glGenBuffers(3, result.VBO);

    // Pass vertex positions as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Pass UVs as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, numUVs * sizeof(float), uvs, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Pass normals as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(float), normals, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glGenBuffers(1, &result.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //glBindVertexArray(0);
    return result;
}