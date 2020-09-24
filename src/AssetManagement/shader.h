#pragma once
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct Shader
{
    unsigned int ID;
    std::unordered_map<std::string, int> uniformLocations;
};

void UseShader(Shader& shader);
void UniformInt(Shader& shader, const char* location, int value);
void UniformFloat(Shader& shader, const char* location, float value);
void UniformVec3(Shader& shader, const char* location, glm::vec3& value);
void UniformVec4(Shader& shader, const char* location, glm::vec4& value);
void UniformMat4(Shader& shader, const char* location, glm::mat4& value);
