#include "Plane.h"

#include <glad/glad.h>
#include <cglm/cglm.h>

static void calc_norm_matrix(Plane_t *plane, Shader_t *shader)
{
	// modelview (mul view and model mtxs to get viewspace positions of normals)
	mat4 vm;
	//glm_mat4_mul(plane->transform, shader->view, &vm);
	glm_mat4_mul(shader->view, plane->transform, &vm);

	// transpose of the inverse of the upper-left 3x3 of the model matrix
	mat3 norm;
	glm_mat3_identity(&norm);
	glm_mat4_pick3(&vm, &norm); // put upper left into norm mtx
	glm_mat3_transpose(&norm);
	glm_mat3_inv(&norm, &norm);
	glm_mat3_copy(&norm, plane->normal_mtx);
}

Plane_t plane_new(float x, float y, float z, float scale)
{
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float planeverts[] = {
		 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,   scale, scale, // top right
		-0.5f, 0.0f, -0.5f,	0.0f, 1.0f, 0.0f, 	0.0f,  scale,  // top left
		 0.5f, 0.0f,  0.5f,	0.0f, 1.0f, 0.0f, 	scale, 0.0f,  // bottom right
		-0.5f, 0.0f,  0.5f,	0.0f, 1.0f, 0.0f, 	0.0f,  0.0f    // bottom left
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeverts), &planeverts, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	Plane_t tmp;
	tmp.position[0] = x;
	tmp.position[1] = y;
	tmp.position[2] = z;
	tmp.scale[0] = scale;
	tmp.scale[1] = scale;
	tmp.scale[2] = scale;
	tmp.axis[0] = 0.0f;
	tmp.axis[1] = 0.0f;
	tmp.axis[2] = 0.0f;
	tmp.angle = 0.0f;
	tmp.material = material_new();
	tmp.VAO = VAO;

	return tmp;
}

void plane_transform(const Plane_t *plane, const Shader_t *shader)
{
	glm_translate(&plane->transform, &plane->position);
	glm_rotate(&plane->transform, glm_rad(plane->angle), &plane->axis);
	glm_scale(&plane->transform, &plane->scale);

	calc_norm_matrix(plane, shader);
	shader_set_model(shader, &plane->transform);
	shader_uniform_mat4fv(shader, &plane->transform, "model");
	shader_uniform_mat3fv(shader, &plane->normal_mtx, "normal_mtx");
	shader_mul(shader);

	glm_mat4_identity(&shader->model);
	glm_mat4_identity(&plane->transform);
}

void plane_draw(const Plane_t *plane, const Shader_t *shader)
{
	shader_use(shader);

	plane_transform(plane, shader);
	material_use(&plane->material, shader);
	glBindVertexArray(plane->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	shader_usei(0);
}