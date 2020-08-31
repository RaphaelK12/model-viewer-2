#version 330 core
in vec2 uvs;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D diffuse;
uniform vec3 pointLightPos;
uniform vec3 cameraPos;

void main()
{
    vec3 lightDir = normalize(pointLightPos - fragPos);
    float diffuseStrength = max(0.0, dot(lightDir, normal));
    vec3 diffuse = texture(diffuse, uvs).xyz * diffuseStrength;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectedDir = reflect(-lightDir, normal);
    float specularStrength = pow(max(0.0, dot(viewDir, reflectedDir)), 32);
    vec3 specular = vec3(1.0) * specularStrength;

    fragColor = vec4(diffuse + specular, 1.0);
}