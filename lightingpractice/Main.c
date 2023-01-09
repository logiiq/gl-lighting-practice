#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "Time.h"

// Game object includes
#include "Cube.h"
#include "Plane.h"

GLFWwindow *window;
Camera_t camera;

Shader_t testShader;
Shader_t lightingShader;

// Game objects
Plane_t groundPlane;
Plane_t roofPlane;


Cube_t cube;
Cube_t cube2;

/* 
* TODO: 
* - Add a normal matrix that is calculated on the CPU for each object
* - (Optional but highly recommended for optimisation) Implement a resource manager for your textures
*/
void init_world(void)
{
	texture_t lanternOn = texture_new("texture/lanterncleannc.png", GL_TRUE, GL_RGBA);
	texture_t lanternOff = texture_new("texture/lanterncleanoff.png", GL_TRUE, GL_RGBA);
	texture_t wood = texture_new("texture/wood.png", GL_TRUE, GL_RGBA);

	// Load objects
	groundPlane = plane_new(0.0f, -0.5f, 0.0f, 20.0f);

	roofPlane = plane_new(0.0f, 5.5f, 0.0f, 20.0f);
	roofPlane.texture = wood;
	roofPlane.axis[0] = 1.0f;
	roofPlane.angle = 180.0f;

	cube = cube_new(-1.5f, 0.0f, 0.0f);
	cube.texture = lanternOn;

	cube2 = cube_new(1.5f, 0.0f, 0.0f);
	cube2.texture = lanternOff;
}

vec3 lightPos = { 0.0f, 2.0f, -2.0f };
float spd = 0.001f;

void draw_world(void)
{
	//lightPos[0] += spd;
	
	if (lightPos[0] >= 10.0f || lightPos[0] <= -10.0)
	{
		spd = -spd;
	}
	
	//lightPos[1] = 2.0f * sinf(0.5f * glfwGetTime()) + 2.5f;
	//lightPos[2] += 0.001f;

	cube.position[0] = lightPos[0];
	cube.position[1] = lightPos[1];
	cube.position[2] = lightPos[2];

	cube.axis[0] = 1.0f;
	cube.axis[1] = 1.0f;
	cube.angle += 0.1f;

	shader_use(&lightingShader);
	shader_uniform3f(&lightingShader, lightPos[0], lightPos[1], lightPos[2], "lightPos");
	shader_usei(0);

	// Draw objects
	plane_draw(&groundPlane, &lightingShader);
	plane_draw(&roofPlane, &lightingShader);

	cube_draw(&cube, &testShader);
	cube_draw(&cube2, &lightingShader);
}

void init(void)
{
	if (!glfwInit())
	{
		printf("Unable to initialze GLFW\n");
		glfwTerminate();
		return -1;
	}

	window = glfwCreateWindow(1920, 1080, "OpenGL Lighting Test", NULL, NULL);

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Unable to initialize GLAD\n");
	}

	glViewport(0, 0, 1920, 1080);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	
	testShader = shader_new("shader/vertex.glsl", "shader/fragment.glsl");
	lightingShader = shader_new("shader/lighting/vertex.glsl", "shader/lighting/fragment.glsl");
	
	
	input_init();
	camera = camera_new(0.0f, 1.0f, 3.0f, 0.0f, -90.0f);

	// load world
	init_world();

	glfwShowWindow(window);
}

void loop(void)
{
	int dayNight = 0;
	float lightValue = 0.5f;
	float currentLightValue = 0.0f;
	vec3 bgColor = { 0.0f, 0.3f, 0.5f };
	vec3 currentColor = { 0.0f, 0.0f, 0.0f };
	int fps = 0;
	float polltime = 0.0f;

	vec3 lightColour = { 0.0f, 0.0f, 0.0f };

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(currentColor[0], currentColor[1], currentColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera_update(&camera, &testShader);
		camera_update(&camera, &lightingShader);
		shader_uniform3fv(&lightingShader, &camera.position, "viewPos");

		// Calculate day/night cycle
		if (dayNight)
		{
			float mul = 0.5f * sinf(0.5f * glfwGetTime()) + 0.5f;
			currentColor[0] = bgColor[0] * mul;
			currentColor[1] = bgColor[1] * mul;
			currentColor[2] = bgColor[2] * mul;
			currentLightValue = lightValue * mul;
		
			shader_use(&lightingShader);
			shader_uniform1f(&lightingShader, currentLightValue, "ambientStrength");
			shader_usei(0);
		}

		// set rgb light color
		lightColour[0] = 0.5f * sinf(glfwGetTime()) + 0.5f;
		lightColour[1] = 0.5f * sinf(glfwGetTime() + (2.0f * M_PI / 3)) + 0.5f;
		lightColour[2] = 0.5f * sinf(glfwGetTime() + (4.0f * M_PI / 3)) + 0.5f;
		shader_use(&lightingShader);
		shader_uniform3fv(&lightingShader, &lightColour, "lightColour1");
		shader_uniform3f(&lightingShader, lightColour[0], lightColour[1], lightColour[2], "lightColour1");
		shader_usei(0);

		shader_use(&testShader);
		shader_uniform3fv(&lightingShader, &lightColour, "lightColour1");
		shader_uniform3f(&testShader, lightColour[0], lightColour[1], lightColour[2], "lightColour1");
		shader_usei(0);

		draw_world();

		glfwSwapBuffers(window);
		glfwPollEvents();
		input_process();

		// Poll FPS
		if (polltime >= 1.0f)
		{
			printf("(DEBUG) FPS: %d\n", fps);
			fps = 0;
			polltime = 0.0f;
		}
		fps++;
		polltime += time_deltaTimef();
		time_calc();
	}
}

void cleanup(void)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(void)
{
	init();
	loop();
	cleanup();
	return 0;
}

const Shader_t *getShader(void)
{
	return &testShader;
}

GLFWwindow *getWindow(void)
{
	return window;
}

const Camera_t *getCamera(void)
{
	return &camera;
}