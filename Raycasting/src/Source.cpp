
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <math.h>

#include "OpenglBufferObjects.h"
#include "Shader.h"
#include "Texture.h"

#include "Geometry.h"
#include "TexturedQuad.h"

#include "yse.hpp"

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

	YSE::System().init();
	YSE::sound mysound;
	mysound.create("assets/ShootingStars.ogg");
	mysound.setLooping(true);
	mysound.play();


	Texture tex1("assets/meme.jpg");
	Texture tex2("assets/pattern.png");
	TexturedQuad tq = TexturedQuad(-0.9, -0.9, 1.8, 1.8, tex1);


	float n = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		n += 0.01f;

		if ((int)n / 5 % 2 == 0)
			tq.setTexture(tex1);
		else
			tq.setTexture(tex2);

		tq.setTextureSampleArea(n/2, n/3, sin(n) + 1.1f, sin(n) + 1.1f);
		tq.render();

		mysound.setPosition(YSE::Vec(sin(n) * 10, 0.0f, cos(n) * 10));
		//mysound.setVolume(1.0f - ((sin(n) * 0.5f) + 0.5f) + 0.1f);
		YSE::System().update();
	}

	tex1.freeMemory();
	tex2.freeMemory();

	YSE::System().close();

	glfwDestroyWindow(window);
	glfwTerminate();
}