
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <math.h>

#include "OpenglBufferObjects.h"
#include "Shader.h"

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

	float vertices[] = {
		0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
		1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		1.0f, 0.0f,   0.0f, 1.0f, 1.0f
	};
	VertexBuffer vb = VertexBuffer(vertices, 5 * 4 * sizeof(float));

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	IndexBuffer ib = IndexBuffer(indices, 6 * sizeof(unsigned int));

	VertexArray va = VertexArray("ff fff", vb, ib);

	std::string vertexShader =
		"#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec3 color;\n"
		"out vec3 v_color;\n"
		"void main() {\n"
		"	gl_Position = vec4(position, 0, 1);\n"
		"	v_color = color;\n"
		"};\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"in vec3 v_color;\n"
		"void main() {\n"
		"	color = vec4(v_color, 1);\n"
		"};\n";

	Shader s = Shader(vertexShader, fragmentShader);
	

	float n = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor((float)sin(n) / 2 + 0.5f, 1, 0, 1);
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		n += 0.01f;

		va.bind();
		s.bind();
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
		va.unbind();
		s.unbind();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}