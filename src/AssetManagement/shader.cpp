#include "shader.h"
#include <glad/glad.h>

void UseShader(Shader& shader)
{
    glUseProgram(shader.ID);
}

void UniformInt(Shader& shader, const char* location, int value) {glUniform1i(shader.uniformLocations[location], value);}
void UniformFloat(Shader& shader, const char* location, float value) {glUniform1f(shader.uniformLocations[location], value);}
void UniformVec3(Shader& shader, const char* location, glm::vec3& value)
{
    glUniform3fv(shader.uniformLocations[location], 1, &value.x);
}
void UniformVec4(Shader& shader, const char* location, glm::vec4& value)
{
    glUniform4fv(shader.uniformLocations[location], 1, &value.x);
}
void UniformMat4(Shader& shader, const char* location, glm::mat4& value)
{
    glUniformMatrix4fv(shader.uniformLocations[location], 1, GL_FALSE, &value[0][0]);
}