
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <math.h>

#include "OpenglBufferObjects.h"
#include "Shader.h"
#include "Texture.h"

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

	float vertices[] = {
		0.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 1.0f,  0.0f, 1.0f,
		1.0f, 1.0f,  1.0f, 1.0f,
		1.0f, 0.0f,  1.0f, 0.0f,
	};
	VertexBuffer vb = VertexBuffer(vertices, 4 * sizeof(float) * 4);

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	IndexBuffer ib = IndexBuffer(indices, 6 * sizeof(unsigned int));

	VertexArray va = VertexArray("ff ff", vb, ib);

	Texture tex = Texture(500, 500);

	std::ifstream stream("texShader.sh");
	Shader sh = Shader(stream);


	float n = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		n += 0.01f;

		tex.bind(0);
		sh.setUniform1i("u_texture", 0);
		sh.bind();
		tex.bind();
		va.bind();
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
		va.unbind();
		tex.unbind();
		sh.unbind();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}