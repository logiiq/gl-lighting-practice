#include "Monitor.h"
#include "Shader.h"
#include <GLFW/glfw3.h>

GLFWwindow *getWindow(void);
GLFWmonitor *monitor;

void windowsizecallback(GLFWwindow *window, int width, int height);

void monitor_init(void)
{
	monitor = glfwGetPrimaryMonitor();
	glfwSetWindowSizeCallback(getWindow(), &windowsizecallback);
}

void windowsizecallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);

	unsigned int nr = 0;
	Shader_t **allshaders = shader_get_all(&nr);

	for (unsigned int i = 0; i < nr; i++)
	{
		shader_viewport_calc(allshaders[i]);
	}

}