#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h> 
#include <cglm/cglm.h>
#include "Shader.h"

#include "Texture.h"

typedef struct Cube
{
	// Cube position
	vec3 position;
	mat4 transform;
	vec3 scale;
	vec3 axis;
	float angle;

	// GL handle
	int VAO;
	texture_t texture;
} Cube_t;

static float cube_verts[] = {
	// Front face		// Normals		  // Tex coords
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Bottom front left
	 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // Bottom front right
	 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top front right
	-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // Top front left

	// Back face
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // Bottom back left
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // Bottom back right
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // Top back right
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // Top back left

	// Left face
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom back left
	-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom front left
	-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top front left
	-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top back left

	// Right face
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Bottom back left
	0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Bottom front left
	0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Top front left
	0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // Top back left

	// Top face
	-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // Top front left
	 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Top front right
	 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // Top back right
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Top back left

	// Bottom face
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Bottom front left
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Bottom front right
	 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Bottom back right
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f   // Bottom back left
};

static unsigned int cube_indices[] = {
	0, 1, 2, 0, 2, 3,
	6, 5, 4, 7, 6, 4,
	8, 9, 10, 8, 10, 11,
	14, 13, 12, 15, 14, 12,
	16, 17, 18, 16, 18, 19,
	22, 21, 20, 23, 22, 20
};

Cube_t cube_new(float x, float y, float z);

void cube_draw(Cube_t *cube, const Shader_t *shader);

#endif