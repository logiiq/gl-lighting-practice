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
}