#include "Shader.h"

#include <glad/glad.h>

static char *buf; // buffer to store strings before compiling shaders

// private function prototypes
static void checkShader(unsigned int shader);
const char *getStringFromFile(const char *file_path);
const unsigned int filelen(FILE *file);

// TODO: shader_t -> Shader_t, needs to be a struct to store the transformations of each matrix

// Create a new shader with vertex shader at vpath and fragment shader at fpath
Shader_t shader_new(const char *vpath, const char *fpath)
{
	// Handles
	unsigned int id;
	unsigned int vsh;
	unsigned int fsh;
	Shader_t tmp;

	// Get source code from file and put in memory
	const char *vsource = getStringFromFile(vpath);
	const char *fsource = getStringFromFile(fpath); 
	
	if (vsource == NULL || fsource == NULL)
	{
		printf("Error loading vertex or fragment shader source code\n");
		tmp.id = 0;
		return tmp;
	}

	// Create GL shader program
	id = glCreateProgram(); 

	// Create shaders
	vsh = glCreateShader(GL_VERTEX_SHADER);
	fsh = glCreateShader(GL_FRAGMENT_SHADER);

	// Bind source to shaders
	glShaderSource(vsh, 1, &vsource, NULL);
	glShaderSource(fsh, 1, &fsource, NULL);
	
	// Compile and check both shaders
	glCompileShader(vsh);
	checkShader(vsh);

	glCompileShader(fsh);
	checkShader(fsh);
	
	// Attach, link and use program
	glAttachShader(id, vsh);
	glAttachShader(id, fsh);
	glLinkProgram(id);
	glUseProgram(id);

	// Cleanup
	glDeleteShader(vsh);
	glDeleteShader(fsh);
	
	// Free malloc'd memory, no need to free buf as buf is pointed to by fsource
	free(vsource);
	vsource = NULL;
	free(fsource);
	fsource = NULL;
	// Null reference buffers
	buf = NULL;

	tmp.id = id;
	shader_init(&tmp);

	int numUniforms;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numUniforms);

	char buf[256];
	int size = 0;
	unsigned int type = 0;

	/*
	printf("Program %d created, detected uniforms:\n", id);
	for (unsigned int i = 0; i < numUniforms; i++)
	{
		glGetActiveUniform(id, i, 256, NULL, &size, &type, &buf);
		printf("Uniform %d Type: %d Name: %s\n", i, type, &buf);
	}
	printf("\n\n");
	*/

	glUseProgram(GL_NONE);

	return tmp;
}

static float degToRad(float f)
{
	return f * (float)M_PI / 180.0f;
}

void shader_init(Shader_t *shader)
{
	glm_mat4_identity(shader->f_matrix);
	glm_mat4_identity(shader->proj);
	glm_mat4_identity(shader->view);
	glm_mat4_identity(shader->model);
	
	// Do transformations
	glm_perspective(degToRad(90.0f), 16.0f / 9.0f, 0.001f, 100.0f, shader->proj);
	
	glUseProgram(shader->id);
	shader_mul(shader);
	glUseProgram(GL_NONE);
}

void shader_viewport_recalc(Shader_t *shader)
{
	// reset proj shader to identity before recalculating
	glm_mat4_identity(shader->proj);
	glm_perspective(degToRad(90.0f), 16.0f / 9.0f, 0.001f, 100.0f, shader->proj);

	glUseProgram(shader->id);
	shader_mul(shader);
	glUseProgram(GL_NONE);
}

/**
* @brief Premultiply all matrixes and send them to the shader program
* @out Shader_t The shader whose matrices to premultiply
*/
void shader_mul(Shader_t *shader)
{	
	glm_mul(shader->proj, shader->view, shader->f_matrix);
	glm_mul(shader->f_matrix, shader->model, shader->f_matrix);
	shader_uniform_mat4fv(shader, shader->f_matrix, "finalMat");
}

void shader_use(const Shader_t *shader)
{
	glUseProgram(shader->id);
}

void shader_usei(const unsigned int shader)
{
	glUseProgram(shader);
}

void shader_set_view(Shader_t *shader, mat4 *v)
{
	glm_mat4_copy(v, shader->view);
}

void shader_set_model(Shader_t *shader, mat4 *m)
{
	glm_mat4_copy(m, shader->model);
}

void shader_uniform1f(const Shader_t *shader, float val, const char *uniform)
{
	int loc = glGetUniformLocation(shader->id, uniform);
	glUniform1f(loc, val);
}

void shader_uniform3f(const Shader_t *shader, float v0, float v1, float v2, const char *uniform)
{
	int loc = glGetUniformLocation(shader->id, uniform);
	glUniform3f(loc, v0, v1, v2);
}

void shader_uniform3fv(const Shader_t *shader, const vec3 *vec, const char *uniform)
{
	int loc = glGetUniformLocation(shader->id, uniform);
	glUniform3fv(loc, 3, vec);
}

void shader_uniform_mat3fv(const Shader_t *shader, mat4 *matrix, const char *uniform)
{
	int loc = glGetUniformLocation(shader->id, uniform);
	glUniformMatrix3fv(loc, 1, GL_FALSE, matrix);
}

void shader_uniform_mat4fv(const Shader_t *shader, mat4 *matrix, const char *uniform)
{
	int loc = glGetUniformLocation(shader->id, uniform); // returns -1 on failure
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
	}
	else
	{
		// Mute errors for now, as not all shaders have the same uniforms
		//printf("ERROR: Uniform variable '%s' not found in shader '%d'\n", uniform, shader->id);
	}
}

static void checkShader(unsigned int shader)
{
	int err = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
	if (!err)
	{
		int len;
		char message[1024];
		glGetShaderInfoLog(shader, 1024, &len, message);
		printf("Shader (%d) Infolog: %s\n", shader, message);
	}
}

const char *getStringFromFile(const char *file_path)
{
	FILE *f;
	size_t size;
	size_t result = 0;

	errno_t error_code = fopen_s(&f, file_path, "rb");

	if (!error_code)
	{
		size = filelen(f); // Get file length

		// Allocate memory to contain the entire file (+1 for the string terminator)
		buf = malloc(sizeof(char) * size + 1);

		if (buf)
		{
			result = fread_s(buf, size, sizeof(char), size, f); // Read file into buf
			fclose(f);
			*(buf + size) = '\0';
		}
		else 
		{
			printf("ERROR: Unable to malloc for shader string buffer\n");
		}

		if (result != size)
		{
			printf("ERROR: Something went wrong\n");
			fclose(f);
			free(buf);
			buf = NULL;
			f = NULL;
			printf("ERROR: Freed memory and set nullptrs\n");
		}
		// printf("%s\n", buf); // debug
		return buf;
	}
	else if (error_code)
	{
		printf("%d: file doesnt exist\n", error_code);
		return NULL;
	}

	return NULL;
}

// Get num elements from a file
// Sets seek position to start after reading
inline const unsigned int filelen(FILE *file)
{
	unsigned int tmp;
	fseek(file, 0, SEEK_END);
	tmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	return tmp;
}

