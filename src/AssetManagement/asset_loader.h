#pragma once
#include <unordered_map>
#include <string>
#include "../Mesh/mesh.h"

struct Shader
{
    unsigned int ID;
    std::unordered_map<std::string, int> uniformLocations;
};

struct Texture
{
    unsigned int width, height, channels;
    unsigned int ID, index;

    static int GlobalTextureIndex;
};

Shader LoadShadersFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath);
Texture LoadTextureFromFile(const char* path);
MeshIndexed LoadMeshIndexedFromOBJ(const char* path);