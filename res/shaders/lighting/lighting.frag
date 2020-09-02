#version 330 core
in vec2 uvs;
in vec3 fragPos_tangentSpace;
in vec3 lightPos_tangentSpace;
in vec3 viewPos_tangentSpace;
out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main()
{
    vec3 normalMapTexture = 2.0 * texture(normalMap, uvs).rgb - 1.0;
    vec3 normal = normalize(normalMapTexture);
    vec3 lightDir = normalize(lightPos_tangentSpace - fragPos_tangentSpace);
    float diffuseStrength = max(0.0, dot(lightDir, normal));
    vec3 diffuse = texture(diffuseMap, uvs).xyz * diffuseStrength;

    vec3 viewDir = normalize(viewPos_tangentSpace - fragPos_tangentSpace);
    vec3 reflectedDir = reflect(-lightDir, normal);
    float specularStrength = pow(max(0.0, dot(viewDir, reflectedDir)), 32);
    vec3 specular = vec3(1.0) * specularStrength;

    fragColor = vec4(diffuse + specular, 1.0);
}