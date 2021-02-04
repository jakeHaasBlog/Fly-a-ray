
#include "game/GameLogicInterface.h"

// this stops the variables declared here from becoming globaly accessable
namespace {
	Camera cam = Camera(0, 0, 0, 1.2, 300);
	std::vector<SeeableEntity*> walls = {};
	BatchLines primativeFOV;
	Texture bqTex;
	Texture bqTex2;
	Texture bqTex3;
	bool mouseEnabled = false;

	BitmapText text;
	BitmapText lotsOfText;
	std::string myText = "This is some sample text...";

	Texture brickTexture("assets/bricks.jfif");

	static std::array<float, 4> green = { 0.3f, 1.0f, 0.3f, 0.7f };
	static std::vector<Prop*> props = {
	};
}

void GameLogicInterface::init() {

	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.setResolution(1920, 1080);


	//outer walls
	walls.push_back(new SeeableLine(1.5f, 1.5f, -1.5f, 1.5f));
	(*(walls.end() - 1))->setColor(0, 0, 1);

	walls.push_back(new SeeableLine(-1.5f, 1.5f, -1.5f, -1.5f));
	(*(walls.end() - 1))->setColor(0, 0, 1);

	walls.push_back(new SeeableLine(-1.5f, -1.5f, 1.5f, -1.5f));
	(*(walls.end() - 1))->setColor(0, 0, 1);

	walls.push_back(new SeeableLine(1.5f, -1.5f, 1.5f, 1.5f));
	(*(walls.end() - 1))->setColor(0, 0, 1);
			

	walls.push_back(new SeeableRectangle(0, 0, .3, .3));
	(*(walls.end() - 1))->setColor(1, 0, 0);


	int numLines = 120;
	for (int i = 0; i < numLines; i++) {
		float x = ((float)i / numLines) * 4 - 2.0f;
		primativeFOV.addLine(x / 2 - 1.0f, 0.9f, x / 2 - 1.0f, -0.9f);
	}


}

// deltaTime is the milliseconds between frames. Use this for calculating movement to avoid slowing down if there is lag 
void GameLogicInterface::update(float deltaTime) {


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// controlls
	if (!glfwJoystickPresent(GLFW_JOYSTICK_1) || !glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		if (window.keyIsDown(GLFW_KEY_W)) {
			float deltaX = cos(cam.getDirection()) * 0.0003f * deltaTime;
			float deltaY = sin(cam.getDirection()) * 0.0003f * deltaTime;
			cam.setX(cam.getX() + deltaX);
			cam.setY(cam.getY() + deltaY);
		}

		if (window.keyIsDown(GLFW_KEY_A)) {
			float deltaX = cos(cam.getDirection() + 3.14159f / 2.0f) * 0.0002f * deltaTime;
			float deltaY = sin(cam.getDirection() + 3.14159f / 2.0f) * 0.0002f * deltaTime;
			cam.setX(cam.getX() + deltaX);
			cam.setY(cam.getY() + deltaY);
		}

		if (window.keyIsDown(GLFW_KEY_D)) {
			float deltaX = cos(cam.getDirection() - 3.14159f / 2.0f) * 0.0002f * deltaTime;
			float deltaY = sin(cam.getDirection() - 3.14159f / 2.0f) * 0.0002f * deltaTime;
			cam.setX(cam.getX() + deltaX);
			cam.setY(cam.getY() + deltaY);
		}

		if (window.keyIsDown(GLFW_KEY_S)) {
			float deltaX = cos(cam.getDirection()) * 0.0002f * deltaTime;
			float deltaY = sin(cam.getDirection()) * 0.0002f * deltaTime;
			cam.setX(cam.getX() - deltaX);
			cam.setY(cam.getY() - deltaY);
		}
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
		if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
	
			GLFWgamepadstate state;
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

			float dir = (-state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] / 20.0f) * (deltaTime / 16.0f);
			if (abs(dir) > 0.01f) {
				cam.setDirection(dir + cam.getDirection());
			}

			float sy = (-state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] / 70.0f) * (deltaTime / 16.0f);
			if (abs(sy) > 0.004f) {
				cam.setX(cos(cam.getDirection()) * sy + cam.getX());
				cam.setY(sin(cam.getDirection()) * sy + cam.getY());
			}

			float sx = (-state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] / 100.0f) * (deltaTime / 16.0f);
			if (abs(sx) > 0.004f) {
				cam.setX(cos(cam.getDirection() + (3.14159f / 2.0f)) * sx + cam.getX());
				cam.setY(sin(cam.getDirection() + (3.14159f / 2.0f)) * sx + cam.getY());
			}

		}
	}






	//world rendering
	ViewportManager::bindViewportNormalized(ViewportManager::getLeftViewportBound(), -1.0f, ViewportManager::getRightViewportBound() - ViewportManager::getLeftViewportBound(), 2.0f);
	cam.renderView(walls, props);
	ViewportManager::unbindViewport();


	//minimap texture
	static Texture minimapTexture(750 / 2, 500 / 2);
	minimapTexture.bindAsRenderTarget();
	Geo::Rectangle::fillRect(ViewportManager::getLeftViewportBound(), ViewportManager::getBottomViewportBound(), ViewportManager::getRightViewportBound() - ViewportManager::getLeftViewportBound(), ViewportManager::getTopViewportBound() - ViewportManager::getBottomViewportBound(), 0, 0, 0);
	cam.renderPrimitiveRays({ -cam.getX(), -cam.getY() }, 1.0f, walls, props);
	for (SeeableEntity* e : walls) {
		e->renderPrimitive({ -cam.getX(), -cam.getY() }, 1.0f);
	}
	minimapTexture.unbindAsRenderTarget();

	// minimap quad
	static TexturedQuad minimapQuad;
	minimapQuad.setTexture(minimapTexture);
	minimapQuad.setX(ViewportManager::getRightViewportBound() - 0.75f);
	minimapQuad.setY(ViewportManager::getBottomViewportBound());
	minimapQuad.setWidth(0.75f);
	minimapQuad.setHeight(0.5f);


	// primitave rays

	static float colorShift = 0.0f;
	colorShift += 0.03f * deltaTime / 16.0f;
	for (int i = 0; i < primativeFOV.size(); i++) {
		float percent = (float)i / primativeFOV.size();
		float colorRotator = colorShift + percent * 5;
		float colorRotator2 = colorShift - percent * 5;

		float r1 = sin(colorRotator / 1) / 2 + 0.5f;
		float g1 = sin(colorRotator / 2) / 2 + 0.5f;
		float b1 = sin(colorRotator / 4) / 2 + 0.5f;

		float r2 = sin(colorRotator2 / 1) / 2 + 0.5f;
		float g2 = sin(colorRotator2 / 2) / 2 + 0.5f;
		float b2 = sin(colorRotator2 / 4) / 2 + 0.5f;

		primativeFOV.setBeginLineColor(i, r1, g1, b1, 1.0f);
		primativeFOV.setEndLineColor(i, r2, g2, b2, 1.0f);

		BatchLine& line = primativeFOV.getLine(i);
		line.y1 = (sin(colorRotator) / 2 + 0.5f) * 0.3 + 0.7f;

	}

	minimapQuad.render();
	
}

void GameLogicInterface::cleanup() {
	for (SeeableEntity* entity : walls) {
		delete entity;
	}
	
	for (Prop* p : props) {
		delete p;
	}
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
		mouseEnabled = !mouseEnabled;
		glfwSetInputMode(window.getHandle(), GLFW_CURSOR, mouseEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	if (key == GLFW_KEY_S && (mods & GLFW_MOD_CONTROL)) {
		Texture myTex = Texture(500, 400);
		myTex.saveToFile("assets/savedImage.png");
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		//explosionSound.setPosition({ cam.getX(), cam.getY() }, cam.getDirection(), { -0.7f, 0.35f });
		//explosionSound.set2D(false);
		//explosionSound.playAll();
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		//typeWriterSound.playRandomPart();
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		//if (loopingMusic.isPlaying()) {
		//	loopingMusic.pause();
		//}
		//else {
		//	loopingMusic.resume();
		//}
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		//loopingMusic.setVolume(loopingMusic.getVolume() + 0.05f);
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		//loopingMusic.setVolume(loopingMusic.getVolume() - 0.05f);
	}

}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}
