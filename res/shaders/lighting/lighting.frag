#version 330 core
in vec2 uvs;
//in vec3 fragPos_tangentSpace;
//in vec3 lightPos_tangentSpace;
//in vec3 viewPos_tangentSpace;

in vec3 fragPos;
in vec3 lightPos;
in vec3 viewPos;
in mat3 TBN;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

void main()
{
    vec3 normalMapTexture = 2.0 * texture(normalMap, uvs).rgb - 1.0;
    vec3 normal = normalize(TBN * normalMapTexture);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuseStrength = max(0.0, dot(lightDir, normal));
    vec3 diffuse = texture(diffuseMap, uvs).xyz * diffuseStrength;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectedDir = reflect(-lightDir, normal);
    float specularStrength = pow(max(0.0, dot(viewDir, reflectedDir)), 32);
    vec3 specular = vec3(texture(specularMap, uvs).r) * specularStrength;

    fragColor = vec4(diffuse + specular, 1.0);
}