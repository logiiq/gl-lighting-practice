#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/cglm.h>
#include "Shader.h"

typedef struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} Light_t;

Light_t light_new(float x, float y, float z);

void light_update(Light_t *light, Shader_t *shader);

void light_position(Light_t *light, float x, float y, float z);

void light_ambient(Light_t *light, float r, float g, float b);

void light_diffuse(Light_t *light, float r, float g, float b);

void light_specular(Light_t *light, float r, float g, float b);

#endif