#version 400 core
out vec4 FragColor;

in vec2 texPos;

uniform sampler2D sampler;
uniform vec3 lightColour1;

void main()
{
    FragColor = vec4(lightColour1, 1.0f) * texture(sampler, texPos);
}