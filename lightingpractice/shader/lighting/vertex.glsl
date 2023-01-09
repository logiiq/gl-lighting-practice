// Lighting vshader
#version 400 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normalPos;
layout (location = 2) in vec2 texturePos;

// # To fragment shader # //
out vec3 normPos;
out vec2 texPos;
out vec3 fragPos;
out vec3 vp;

uniform mat4 finalMat;
uniform mat4 model;
uniform vec3 viewPos;

void main()
{
    gl_Position = finalMat * vec4(vertexPos, 1.0f);
    fragPos = vec3(model * vec4(vertexPos, 1.0f));
    normPos = vec3(vec4(normalPos, 0.0f) * model);
    vp = viewPos;

    texPos = texturePos;
}