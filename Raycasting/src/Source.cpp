
#include <stdio.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <math.h>

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


	// making a vertex buffer
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLfloat vertices[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// making an index buffer
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// making a vertex array
	static int attribsCount = 0;
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glEnableVertexAttribArray(attribsCount);
	glVertexAttribPointer(attribsCount, 2, GL_FLOAT, false, 2 * sizeof(float), (const void*)0);
	glBindVertexArray(0);



	float n = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor((float)sin(n) / 2 + 0.5f, 1, 0, 1);
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		n += 0.01f;

		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}