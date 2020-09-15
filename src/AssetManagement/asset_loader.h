#pragma once
#include "shader.h"
#include "texture.h"
#include "../Mesh/mesh.h"

struct Model
{
    Mesh mesh;
    Texture diffuse;
    Texture normal;
    Texture specular;
};

struct Shader LoadShadersFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath);
Texture LoadTextureFromFile(const char* path);
Texture LoadCubemapFromFiles(const char* folderPath);
Mesh LoadMeshFromOBJ(const char* path);
MeshIndexed LoadMeshIndexedFromOBJ(const char* path);