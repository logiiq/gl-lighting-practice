#ifndef PLANE_H
#define PLANE_H

#include <cglm/cglm.h>
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

typedef struct Plane
{
	// positional values
	vec3 position;
	vec3 scale;
	vec3 axis;
	mat4 transform;
	mat3 normal_mtx;
	float angle;

	Material_t material;

	// GL handles
	unsigned int VAO;
} Plane_t;

// ccw winding
static unsigned int indices[] = {
	0, 3, 2,
	0, 1, 3
};

Plane_t plane_new(float x, float y, float z, float scale);

void plane_draw(Plane_t *plane, const Shader_t *shader);

#endif