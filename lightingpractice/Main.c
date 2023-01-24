#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Monitor.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "Time.h"

// Lighting
#include "Light.h"
#include "DirectionalLight.h"
#include "Material.h"

// Game object includes
#include "Cube.h"
#include "Plane.h"

GLFWwindow *window;
Camera_t camera;

Shader_t testShader;
Shader_t lightingShader;

// Lights
Light_t light;

// Game objects
Plane_t groundPlane;
Plane_t roofPlane;

Plane_t wall1;
Plane_t wall2;
Plane_t wall3;
Plane_t wall4;

Cube_t cube;
Cube_t cube2;

static texture_t defaultTexture;

const texture_t texture_default(void)
{
	return defaultTexture;
}

/* 
* TODO: 
* - (Optional but highly recommended for optimisation) Implement a resource manager for your textures
*/
void init_world(void)
{
	defaultTexture = texture_new("texture/def.png", GL_TRUE, GL_RGBA);
	texture_t lanternOn = texture_new("texture/lanternclean.png", GL_TRUE, GL_RGBA);
	texture_t lanternOff = texture_new("texture/lanterncleanoff.png", GL_TRUE, GL_RGBA);
	texture_t wood = texture_new("texture/wood.png", GL_TRUE, GL_RGBA);
	texture_t stone = texture_new("texture/stone.png", GL_TRUE, GL_RGBA);
	light = light_new(0.0f, 0.0f, 0.0f);
	light_ambient(&light, 0.2f, 0.2f, 0.2f);
	light_diffuse(&light, 1.0f, 1.0f, 1.0f);

	
	wall1 = plane_new(-10.0f, 0.0f, 0.0f, 20.0f);
	wall1.axis[2] = 1.0f;
	wall1.angle = -90.0f;

	wall2 = plane_new(10.0f, 0.0f, 0.0f, 20.0f);
	wall2.axis[2] = 1.0f;
	wall2.angle = 90.0f;

	wall3 = plane_new(0.0f, 0.0f, -10.0f, 20.0f);
	wall3.axis[0] = 1.0f;
	wall3.angle = 90.0f;

	wall4 = plane_new(0.0f, 0.0f, 10.0f, 20.0f);
	wall4.axis[0] = 1.0f;
	wall4.angle = -90.0f;


	/*
	roofPlane = plane_new(0.0f, 5.5f, 0.0f, 20.0f);
	roofPlane.texture = stone;
	roofPlane.axis[0] = 1.0f;
	roofPlane.angle = 180.0f;
	*/

	// Load objects
	groundPlane = plane_new(0.0f, -0.5f, 0.0f, 20.0f);
	material_specular(&groundPlane.material, 1.0f, 1.0f, 1.0f);

	cube = cube_new(-1.5f, 0.0f, 0.0f);
	material_diffuse(&cube.material, lanternOn);

	cube2 = cube_new(1.5f, 0.0f, 0.0f);
	material_diffuse(&cube2.material, lanternOff);
	material_specular(&cube2.material, 0.5f, 0.5f, 0.5f);
	material_shininess(&cube2.material, 128.0f);
}

float spd = 0.001f;

void draw_world(void)
{
	if (light.position[0] >= 9.5f || light.position[0] <= -9.5f)
	{
		spd = -spd;
	}
	light.position[0] += spd;
	light.position[1] = 2.0f * sinf(0.5f * glfwGetTime()) + 2.5f;
	light.position[2] = 2.0f;

	cube.position[0] = light.position[0];
	cube.position[1] = light.position[1];
	cube.position[2] = light.position[2];

	cube.axis[0] = 1.0f;
	cube.axis[1] = 1.0f;
	cube.angle += 0.1f;

	shader_use(&lightingShader);
	shader_uniform3fv(&lightingShader, &light.position, "aLightPos");
	shader_usei(0);

	// Draw objects
	plane_draw(&groundPlane, &lightingShader);
	//plane_draw(&roofPlane, &lightingShader);
	plane_draw(&wall1, &lightingShader);
	plane_draw(&wall2, &lightingShader);
	plane_draw(&wall3, &lightingShader);
	plane_draw(&wall4, &lightingShader);

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	glfwSwapInterval(0);
	glViewport(0, 0, 1920, 1080);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	testShader = shader_new("shader/vertex.glsl", "shader/fragment.glsl");
	shader_init(&testShader);

	lightingShader = shader_new("shader/lighting/vertex.glsl", "shader/lighting/fragment.glsl");
	shader_init(&lightingShader);

	monitor_init();
	input_init();
	camera = camera_new(0.0f, 1.0f, 3.0f, 0.0f, -90.0f);

	// load world
	init_world();

	glfwShowWindow(window);
}

void loop(void)
{
	char dayNight = 1;
	vec3 bgColor = { 0.0f, 0.3f, 0.5f };
	vec3 currentColor = { 0.0f, 0.0f, 0.0f };
	int fps = 0;
	float polltime = 0.0f;

	vec3 lightDir = { -0.5f, -1.0f, 0.0f };

	DirectionalLight_t dirLight = light_directional_new(0.5f, -0.73f, -0.32f);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(currentColor[0], currentColor[1], currentColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera_update(&camera, &testShader);
		camera_update(&camera, &lightingShader);
		shader_uniform3fv(&lightingShader, camera.position, "viewPos");

		// Calculate day/night cycle
		if (dayNight)
		{
			float mul = 0.5f * sinf(0.2f * glfwGetTime()) + 0.5f;
			currentColor[0] = bgColor[0] * mul;
			currentColor[1] = bgColor[1] * mul;
			currentColor[2] = bgColor[2] * mul;
			
			light_directional_ambient(&dirLight, 0.3f * mul, 0.3f * mul, 0.3f * mul);
			light_directional_diffuse(&dirLight, 0.3f * mul, 0.3f * mul, 0.3f * mul);
			light_directional_specular(&dirLight, 0.3f * mul, 0.3f * mul, 0.3f * mul);
			light_directional_update(&dirLight, &lightingShader);
		}

		light_update(&light, &lightingShader);
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
	shader_free();
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

GLFWwindow *getWindow(void)
{
	return window;
}

const Camera_t *getCamera(void)
{
	return &camera;
}