#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
// TODO: Bitangents might not be needed as an attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 pointLightPos;
uniform vec3 cameraPos;

out vec2 uvs;
out vec3 fragPos_tangentSpace;
out vec3 lightPos_tangentSpace;
out vec3 viewPos_tangentSpace;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = inverse(mat3(T, B, N));

    uvs = aUVs;
    lightPos_tangentSpace = TBN * pointLightPos;
    viewPos_tangentSpace = TBN * cameraPos;
    fragPos_tangentSpace = TBN * vec3(model * vec4(aPos, 0.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}