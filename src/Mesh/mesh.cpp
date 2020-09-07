#include "mesh.h"
#include <glad/glad.h>

void Draw(Mesh& mesh)
{
    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
}

void DrawLines(Mesh& mesh)
{
    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_LINES, 0, mesh.numVertices);
}

void Draw(MeshIndexed& mesh)
{
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.numVertices, GL_UNSIGNED_INT, nullptr);
}

Mesh GenerateMesh(std::vector<Vec3f>& vertices, std::vector<Vec2f>& uvs, std::vector<Vec3f>& normals, std::vector<Vec3f>& tangents, std::vector<Vec3f>& bitangents)
{
    Mesh result;
    result.numVertices = (unsigned int)vertices.size();

    glGenVertexArrays(1, &result.VAO);
    glBindVertexArray(result.VAO);
    glGenBuffers(5, result.VBO);

    // Pass vertex positions as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, (int)vertices.size() * sizeof(Vec3f), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass UVs as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, (int)uvs.size() * sizeof(Vec2f), &uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass normals as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (int)normals.size() * sizeof(Vec3f), &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass tangents as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, (int)tangents.size() * sizeof(Vec3f), &tangents[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass bitangents as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, (int)bitangents.size() * sizeof(Vec3f), &bitangents[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    return result;
}

MeshIndexed GenerateMeshIndexed(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices, std::vector<Vec2f>& uvs, std::vector<Vec3f>& normals, std::vector<Vec3f>& tangents, std::vector<Vec3f>& bitangents)
{
    MeshIndexed result;
    result.numVertices = (unsigned int)indices.size();

    glGenVertexArrays(1, &result.VAO);
    glBindVertexArray(result.VAO);
    glGenBuffers(5, result.VBO);

    // Pass vertex positions as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, (int)vertices.size() * sizeof(Vec3f), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass UVs as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, (int)uvs.size() * sizeof(Vec2f), &uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass normals as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (int)normals.size() * sizeof(Vec3f), &normals[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass tangents as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, (int)tangents.size() * sizeof(Vec3f), &tangents[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass bitangents as attribute
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, (int)bitangents.size() * sizeof(Vec3f), &bitangents[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Pass indices
    glGenBuffers(1, &result.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    return result;
}

Mesh GenerateCube()
{
    float vertices[]
	{
		// front
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        // back
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        // bottom
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,

        // top
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,

        // left
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // right
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
	};

    Mesh result;
    result.numVertices = 36;
    
    glGenVertexArrays(1, &result.VAO);
    glBindVertexArray(result.VAO);
    
    glGenBuffers(1, &result.VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    return result;
}

Mesh GenerateAxes()
{
    float vertices[]
    {
        // x axis
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        // y axis
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        // z axis
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    Mesh result;
    result.numVertices = 6;

    glGenVertexArrays(1, &result.VAO);
    glBindVertexArray(result.VAO);

    glGenBuffers(1, &result.VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    return result;
}
