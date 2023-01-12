// Lighting vshader
#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexturePos;

// # To fragment shader # //
out vec3 normPos;
out vec2 texPos;
out vec3 fragPos;
out vec3 lightPos;

uniform mat4 finalMat;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normal_mtx;

uniform vec3 aLightPos;

void main()
{
    gl_Position = finalMat * vec4(aPos, 1.0f);
    fragPos = vec3(view * model * vec4(aPos, 1.0f)); // transform fragment to viewspace
    normPos = normal_mtx * aNormal;
    lightPos = vec3(view * vec4(aLightPos, 1.0f)); // transform worldspace to viewspace position
    texPos = aTexturePos;
}