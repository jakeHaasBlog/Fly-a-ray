
#include <stdio.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <math.h>

int main(int argc, char** argv) {

	if (!glfwInit())
		printf("GLFW did not initialize properly\n");

	if (!glewInit())
		printf("GLEW did not initialize properly\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Ray Casting Project", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	float n = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor((float)sin(n) / 2 + 0.5f, 1, 0, 1);
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		n += 0.01f;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}