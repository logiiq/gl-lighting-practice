#version 460 core
out vec4 FragColor;

in vec2 texPos;

uniform sampler2D sampler;
uniform bool shaderDebug;

void main()
{
    vec4 result;

    if(!shaderDebug)
    {
        result = texture(sampler, texPos);
    }
    else
    {
        result = vec4(0.3f, 0.2f, 0.1f, 0.3f);
    }

    FragColor = result;
}