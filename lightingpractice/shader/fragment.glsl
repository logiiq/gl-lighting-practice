#version 400 core
out vec4 FragColor;

in vec2 texPos;

uniform sampler2D sampler;

void main()
{
    FragColor = texture(sampler, texPos);
}