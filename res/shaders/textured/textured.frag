#version 330 core
in vec2 uvs;
out vec4 fragColor;

uniform sampler2D diffuse;

void main()
{
    fragColor = texture(diffuse, uvs);
}