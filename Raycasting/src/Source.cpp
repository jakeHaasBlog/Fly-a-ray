
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <math.h>

#include "OpenglBufferObjects.h"
#include "Shader.h"

#include "Geometry.h"

int main(int argc, char** argv) {

	if (!glfwInit())
		printf("GLFW did not initialize properly\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Ray Casting Project", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		printf("GLEW did not initialize properly\n");

	Geo::Rectangle rect = Geo::Rectangle(0, 0, 0.2, 0.2);
	Geo::Circle circle = Geo::Circle(-0.5, 0.5, 0.1);
	Geo::LineSeg line = Geo::LineSeg(0, 0, -0.5, 0.5);

	float n = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		n += 0.01f;

		rect.renderFilled(1, 0, 0);
		rect.renderOutline(1, 1, 0);

		circle.renderFilled(1, 0, 0);
		circle.renderOutline(1, 1, 0);

		line.render(1, 0, 0);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}