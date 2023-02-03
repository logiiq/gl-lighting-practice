#include "Cube.h"
#include "Camera.h"
#include "Shader.h"

#include <GLFW/glfw3.h>

#include "Time.h"

const Camera_t *getCamera(void);

static void calc_norm_matrix(Cube_t *cube, Shader_t *shader)
{
	// modelview (mul view and model mtxs to get viewspace positions of normals)
	mat4 vm;
	//glm_mat4_mul(cube->transform, shader->view, &vm);
	glm_mat4_mul(shader->view, cube->transform, &vm);

	// transpose of the inverse of the upper-left 3x3 of the model matrix
	mat3 norm;
	glm_mat4_pick3(&vm, &norm); // put upper left into norm mtx
	glm_mat3_inv(&norm, &norm);
	glm_mat3_transpose(&norm);
	glm_mat3_copy(&norm, cube->normal_mtx);
}

Cube_t cube_new(float x, float y, float z)
{
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), &cube_verts, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), &cube_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	Cube_t tmp;
	cube_pos(&tmp, x, y, z);
	cube_scale(&tmp, 1.0f);
	cube_axis(&tmp, 0.0f, 0.0f, 0.0f);
	tmp.angle = 0.0f;
	tmp.material = material_new();
	tmp.VAO = VAO;

	return tmp;
}

void cube_pos(Cube_t *cube, float x, float y, float z)
{
	cube->position[0] = x;
	cube->position[1] = y;
	cube->position[2] = z;
}

void cube_posv(Cube_t *cube, vec3 *position)
{
	glm_vec3_copy(position, cube->position);
}

void cube_scale(Cube_t *cube, float scalar)
{
	glm_vec3_fill(cube->scale, scalar);
}

void cube_axis(Cube_t *cube, float x, float y, float z)
{
	cube->axis[0] = x;
	cube->axis[1] = y;
	cube->axis[2] = z;
}

void cube_transform(const Cube_t *cube, Shader_t *shader)
{
	glm_translate(&cube->transform, &cube->position);
	glm_rotate(&cube->transform, glm_rad(cube->angle), &cube->axis);
	glm_scale(&cube->transform, cube->scale);

	calc_norm_matrix(cube, shader);
	shader_set_model(shader, &cube->transform);
	shader_uniform_mat4fv(shader, &cube->transform, "model");
	shader_uniform_mat3fv(shader, &cube->normal_mtx, "normal_mtx");
	shader_mul(shader);

	glm_mat4_identity(&shader->model);
	glm_mat4_identity(&cube->transform);
}

void cube_draw(const Cube_t *cube, const Shader_t *shader)
{
	shader_use(shader);
	
	cube_transform(cube, shader);

	material_use(&cube->material, shader); // apply material
	glBindVertexArray(cube->VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	shader_usei(0);
}