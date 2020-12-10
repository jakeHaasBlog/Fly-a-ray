
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "engine/Window.h"
#include "game/GameLogicInterface.h"

int main(int argc, char** argv) {
	if (!glfwInit()) printf("GLFW did not initialize properly\n");
	window; // calls the constructor of window and loadis it into static memory
	if (glewInit() != GLEW_OK) printf("GLEW did not initialize properly\n");
	YSE::System().init();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	window.setResolution(1920, 1080);
	GameLogicInterface::init();
	window.mainUpdateLoop();
	GameLogicInterface::cleanup();

	YSE::System().close();
	glfwDestroyWindow(window.getHandle());
	glfwTerminate();
}