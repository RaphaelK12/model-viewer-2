#version 330 core
in vec2 uvs;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D diffuse;
uniform vec3 pointLightPos;

void main()
{
    vec3 lightDir = normalize(pointLightPos - fragPos);
    vec3 diffuse = texture(diffuse, uvs).xyz;
    float diffuseStrength = max(0.0, dot(lightDir, normal));

    fragColor = vec4(diffuseStrength * diffuse, 1.0);
}