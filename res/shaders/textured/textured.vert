#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uvs;

void main()
{
    uvs = aUVs;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}