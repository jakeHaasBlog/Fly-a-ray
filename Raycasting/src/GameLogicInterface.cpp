
#include "GameLogicInterface.h"

// this stops the variables declared here from becoming globaly accessable
namespace {
	Camera cam = Camera(0, 0, 0, (3.14159f * 2.0f) / 5.0f, 30);
	std::vector<SeeableEntity> walls = {};
}

void GameLogicInterface::init() {

	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	walls.push_back(SeeableEntity(1.5f, 0.9f, 1.5f, -0.9f));
	walls.push_back(SeeableEntity(-1.5f, 0.9f, -1.5f, -0.9f));
	walls.push_back(SeeableEntity(-1.5f, 0.9f, 1.5f, 0.9f));
	walls.push_back(SeeableEntity(-1.5f, -0.9f, 1.5f, -0.9f));

	walls.push_back(SeeableEntity(-1.5f, 0.0f, -0.75f, 0.0f));
	walls.push_back(SeeableEntity(-0.75f, -0.9f, -0.75f, -0.2f));

	walls.push_back(SeeableEntity(-0.55f, 0.0f, -0.15f, 0.0f));
	walls.push_back(SeeableEntity(-0.15f, 0.0f, -0.15f, 0.5f));
	walls.push_back(SeeableEntity(-0.15f, 0.5f, -0.25f, 0.5f));
	walls.push_back(SeeableEntity(-0.25f, 0.5f, -0.25, 0.9f));

	walls.push_back(SeeableEntity(0.2f, 0.9f, 0.2f, 0.2f));
	walls.push_back(SeeableEntity(0.2f, 0.0f, 1.5f, 0.0f));
	walls.push_back(SeeableEntity(1.0f, 0.0f, 1.0f, 0.5f));

	walls.push_back(SeeableEntity(0.75f, 0.0f, 0.75f, -0.7f));
	walls.push_back(SeeableEntity(0.75f, -0.5f, 1.0f, -0.5f));
	walls.push_back(SeeableEntity(0.75f, -0.7f, 1.3f, -0.7f));

}

// deltaTime is the milliseconds between frames. Use this for calculating movement to avoid slowing down if there is lag 
void GameLogicInterface::update(float deltaTime) {

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (window.keyIsDown(GLFW_KEY_W)) {
		float deltaX = cos(cam.getDirection()) * 0.0003f * deltaTime;
		float deltaY = sin(cam.getDirection()) * 0.0003f * deltaTime;
		cam.setX(cam.getX() + deltaX);
		cam.setY(cam.getY() + deltaY);
	}

	int height = 0.3f * window.getHeight();
	int width = height * window.getAspectRatio();
	int beginX = window.getWidth() - width;
	glViewport(beginX, 0, width, height);
	cam.renderPrimitiveRays({ -cam.getX(), -cam.getY() }, 1.0f, walls);
	for (SeeableEntity& e : walls) {
		e.renderPrimitive({ -cam.getX(), -cam.getY() }, 1.0f);
	}

	YSE::System().update();
}

void GameLogicInterface::cleanup() {

}

void GameLogicInterface::mouseMoveCallback(double xPos, double yPos)
{
	static float previousX = xPos, previousY = yPos;
	float deltaX = (xPos - previousX) / window.getWidth();
	float deltaY = (yPos - previousY) / window.getHeight();
	previousX = xPos;
	previousY = yPos;

	cam.setDirection(cam.getDirection() - deltaX);

}

void GameLogicInterface::mouseButtonCallback(int button, int action, int mods)
{

}

void GameLogicInterface::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		window.close();
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}
