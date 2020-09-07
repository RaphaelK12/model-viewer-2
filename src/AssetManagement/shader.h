#pragma once
#include <unordered_map>
#include <string>

struct Shader
{
    unsigned int ID;
    std::unordered_map<std::string, int> uniformLocations;
};

void UseShader(Shader& shader);
void UniformInt(Shader& shader, const char* location, int value);
void UniformFloat(Shader& shader, const char* location, float value);
void UniformVec3(Shader& shader, const char* location, struct Vec3f& value);
void UniformVec4(Shader& shader, const char* location, struct Vec4f& value);
void UniformMat4(Shader& shader, const char* location, struct Mat4f& value);
