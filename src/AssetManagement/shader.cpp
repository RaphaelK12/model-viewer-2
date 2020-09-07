#include "shader.h"
#include <glad/glad.h>
#include "../Math/vec3.h"
#include "../Math/vec4.h"
#include "../Math/mat4.h"

void UseShader(Shader& shader)
{
    glUseProgram(shader.ID);
}

void UniformInt(Shader& shader, const char* location, int value) {glUniform1i(shader.uniformLocations[location], value);}
void UniformFloat(Shader& shader, const char* location, float value) {glUniform1f(shader.uniformLocations[location], value);}
void UniformVec3(Shader& shader, const char* location, Vec3f& value)
{
    glUniform3fv(shader.uniformLocations[location], 1, &value.x);
}
void UniformVec4(Shader& shader, const char* location, Vec4f& value)
{
    glUniform4fv(shader.uniformLocations[location], 1, &value.x);
}
void UniformMat4(Shader& shader, const char* location, Mat4f& value)
{
    glUniformMatrix4fv(shader.uniformLocations[location], 1, GL_FALSE, &value[0].x);
}