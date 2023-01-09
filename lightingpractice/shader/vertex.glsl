#version 400 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normalPos;
layout (location = 2) in vec2 texturePos;

uniform mat4 model;
uniform mat4 finalMat;

// To fragment shader
out vec2 texPos;

void main()
{
    gl_Position = finalMat * vec4(vertexPos, 1.0f);
    texPos = texturePos;
}