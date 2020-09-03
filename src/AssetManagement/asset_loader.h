#pragma once
#include "shader.h"
#include "texture.h"
#include "../Mesh/mesh.h"

Shader LoadShadersFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath);
Texture LoadTextureFromFile(const char* path);
Mesh LoadMeshFromOBJ(const char* path);
MeshIndexed LoadMeshIndexedFromOBJ(const char* path);