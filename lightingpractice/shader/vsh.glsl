#version 400 core
layout (location = 0) in vec3 vertexPos;

uniform mat4 finalMat;
uniform float model;

out vec2 texPos;

void main()
{
	float test = model;
	gl_Position = finalMat * vec4(vertexPos, 1.0f);
	//texPos = texturePos;
}