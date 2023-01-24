#include "Light.h"

#include <cglm/cglm.h>
#include "Shader.h"

Light_t light_new(float x, float y, float z)
{
	Light_t tmp;
	light_position(&tmp, x, y, z);
	light_ambient(&tmp, 1.0f, 1.0f, 1.0f);
	light_diffuse(&tmp, 1.0f, 1.0f, 1.0f);
	light_specular(&tmp, 1.0f, 1.0f, 1.0f);
	return tmp;
}

void light_update(Light_t *light, Shader_t *shader)
{
	shader_use(shader);

	vec3 pos;
	glm_mat4_mulv3(&shader->view, &light->position, 1.0f, &pos);

	shader_uniform3fv(shader, &pos, "plights[0].position");
	shader_uniform3fv(shader, &light->ambient, "plights[0].ambient");
	shader_uniform3fv(shader, &light->diffuse, "plights[0].diffuse");
	shader_uniform3fv(shader, &light->specular, "plights[0].specular");
	shader_usei(0);
}

void light_position(Light_t *light, float x, float y, float z)
{
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
}

void light_ambient(Light_t *light, float r, float g, float b)
{
	light->ambient[0] = r;
	light->ambient[1] = g;
	light->ambient[2] = b;
}

void light_diffuse(Light_t *light, float r, float g, float b)
{
	light->diffuse[0] = r;
	light->diffuse[1] = g;
	light->diffuse[2] = b;
}

void light_specular(Light_t *light, float r, float g, float b)
{
	light->specular[0] = r;
	light->specular[1] = g;
	light->specular[2] = b;
}