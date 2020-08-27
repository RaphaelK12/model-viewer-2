#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uvs;
out vec3 normal;
out vec3 fragPos;

void main()
{
    uvs = aUVs;
    normal = mat3(transpose(inverse(model))) * aNormal;
    normal = normalize(normal);
    fragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}