#ifndef PLANE_H
#define PLANE_H

#include <cglm/cglm.h>
#include "Shader.h"
#include "Texture.h"

typedef struct Plane
{
	// positional values
	vec3 position;
	vec3 scale;
	vec3 axis;
	mat4 transform;

	float angle;

	// GL handles
	unsigned int VAO;
	texture_t texture;
} Plane_t;

// ccw winding
static unsigned int indices[] = {
	0, 3, 2,
	0, 1, 3
};

Plane_t plane_new(float x, float y, float z, float scale);

void plane_draw(Plane_t *plane, const Shader_t *shader);

#endif