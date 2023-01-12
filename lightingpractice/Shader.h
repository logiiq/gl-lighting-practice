#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cglm/cglm.h>

typedef struct Shader
{
	unsigned int id;
	mat4 proj;
	mat4 view;
	mat4 model;

	mat4 f_matrix;
} Shader_t;

Shader_t **shader_get_all(unsigned int *nr_shaders);

void shader_free(void);

Shader_t shader_new(const char *vpath, const char *fpath);

void shader_viewport_calc(Shader_t *shader);

void shader_init(Shader_t *shader);

void shader_mul(Shader_t *shader);

void shader_use(const Shader_t *shader);

void shader_usei(const unsigned int shader);

void shader_set_view(Shader_t *shader, mat4 *v);

void shader_set_model(Shader_t *shader, mat4 *m);

void shader_uniform1f(const Shader_t *shader, float val, const char *uniform);

void shader_uniform3f(const Shader_t *shader, float v0, float v1, float v2, const char *uniform);

void shader_uniform3fv(const Shader_t *shader, vec3 *vec, const char *uniform);

void shader_uniform_mat3fv(const Shader_t *shader, mat4 *matrix, const char *uniform);

void shader_uniform_mat4fv(const Shader_t *shader, mat4 *matrix, const char *uniform);

const int shader_num_shaders(void);

#endif