#include "game/GameLogicInterface.h"
#include "game/Enemy.h";
#include "game/Brazier.h"

// this stops the variables declared here from becoming globaly accessable
namespace {
	Camera cam = Camera(0, -1, 0, 1.2, 300);
	std::vector<SeeableEntity*> walls = {};
	BatchLines primativeFOV;
	bool mouseEnabled = false;

	Texture brickTexture("assets/bricks.jfif");
	Texture grimeTexture("assets/Grime.png");
	Texture Badie("assets/Skull.png");


	Enemy BadGuy(.75f, .75f, .25f, .75f, Badie);

	static std::vector<Prop*> props = {};

}

void GameLogicInterface::init() {

	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	window.setResolution(1920, 1080);


	//outer walls

	walls.push_back(new SeeableRectangle(-1.5,-1.5,3,3,&grimeTexture));
	



	int numLines = 120;
	for (int i = 0; i < numLines; i++) {
		float x = ((float)i / numLines) * 4 - 2.0f;
		primativeFOV.addLine(x / 2 - 1.0f, 0.9f, x / 2 - 1.0f, -0.9f);
	}


	//skull
	Prop* badPtr = &BadGuy;
	props.push_back(badPtr);


	int width = 320;
	static AnimatedSprite BrazierAnim("assets/Brazier.png");
	BrazierAnim.addFrame(80, 0 * width, 0 * width, width, width);
	BrazierAnim.addFrame(80, 1 * width, 0 * width, width, width);
	BrazierAnim.addFrame(80, 2 * width, 0 * width, width, width);
	BrazierAnim.addFrame(80, 3 * width, 0 * width, width, width);
	BrazierAnim.addFrame(80, 4 * width, 0 * width, width, width);
	BrazierAnim.addFrame(80, 5 * width, 0 * width, width, width);
	BrazierAnim.addFrame(80, 6 * width, 0 * width, width, width);



	Prop* brazierProp = new Brazier(0.f, 0.f, .25f, .75f, BrazierAnim);

	props.push_back(brazierProp);


}

// deltaTime is the milliseconds between frames. Use this for calculating movement to avoid slowing down if there is lag 
void GameLogicInterface::update(float deltaTime) {


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// controlls
	if (!glfwJoystickPresent(GLFW_JOYSTICK_1) || !glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		if (window.keyIsDown(GLFW_KEY_W)) {
			float deltaX = cos(cam.getDirection()) * 0.0006f * deltaTime;
			float deltaY = sin(cam.getDirection()) * 0.0006f * deltaTime;
			cam.setX(cam.getX() + deltaX);
			cam.setY(cam.getY() + deltaY);
		}

		if (window.keyIsDown(GLFW_KEY_A)) {
			float deltaX = cos(cam.getDirection() + 3.14159f / 2.0f) * 0.0004f * deltaTime;
			float deltaY = sin(cam.getDirection() + 3.14159f / 2.0f) * 0.0004f * deltaTime;
			cam.setX(cam.getX() + deltaX);
			cam.setY(cam.getY() + deltaY);
		}

		if (window.keyIsDown(GLFW_KEY_D)) {
			float deltaX = cos(cam.getDirection() - 3.14159f / 2.0f) * 0.0004f * deltaTime;
			float deltaY = sin(cam.getDirection() - 3.14159f / 2.0f) * 0.0004f * deltaTime;
			cam.setX(cam.getX() + deltaX);
			cam.setY(cam.getY() + deltaY);
		}

		if (window.keyIsDown(GLFW_KEY_S)) {
			float deltaX = cos(cam.getDirection()) * 0.0004f * deltaTime;
			float deltaY = sin(cam.getDirection()) * 0.0004f * deltaTime;
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


	//updates
	BadGuy.Update(deltaTime);

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
	// primitave rays for minimap
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
		entity = nullptr;
	}

	for (Prop* p : props) {
		delete p;
		p = nullptr;
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
		// takes a screenshot
		mouseEnabled = !mouseEnabled;
		glfwSetInputMode(window.getHandle(), GLFW_CURSOR, mouseEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	if (key == GLFW_KEY_S && (mods & GLFW_MOD_CONTROL)) {
		window.getFramebufferTexture().saveToFile("assets/savedImage.png");
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


	static Shader shader1 = Shader(
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 uvCoord;\n"
		"\n"
		"out vec2 v_uvCoord;"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position, 0, 1);\n"
		"	v_uvCoord = uvCoord;\n"
		"};\n"
		,
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"in vec2 v_uvCoord;"
		"uniform sampler2D u_texture;"
		"\n"
		"void main()\n"
		"{\n"
		"	vec4 c = texture(u_texture, v_uvCoord);\n"
		"   color = vec4(c[2], c[1], c[0], c[3]);\n"
		"};\n"
	);

	static Shader shader2 = Shader(
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 uvCoord;\n"
		"\n"
		"out vec2 v_uvCoord;"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position, 0, 1);\n"
		"	v_uvCoord = uvCoord;\n"
		"};\n"
		,
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"in vec2 v_uvCoord;"
		"uniform sampler2D u_texture;"
		"\n"
		"void main()\n"
		"{\n"
		"	color = texture(u_texture, v_uvCoord);\n"
		"   color[0] = 1.0f - color[0];\n"
		"   color[1] = 1.0f - color[1];\n"
		"   color[2] = 1.0f - color[2];\n"
		"};\n"
	);

	static Shader shader3 = Shader(
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 uvCoord;\n"
		"\n"
		"out vec2 v_uvCoord;"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position, 0, 1);\n"
		"	v_uvCoord = uvCoord;\n"
		"};\n"
		,
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"in vec2 v_uvCoord;"
		"uniform sampler2D u_texture;"
		"\n"
		"void main()\n"
		"{\n"
		"	color = texture(u_texture, v_uvCoord);\n"
		"   float intensity = (color[0] + color[1] + color[2]) / 3;\n"
		"   color = vec4(0.0f, intensity * 1 + 0.2, 0.0f, color[3]);\n"
		"};\n"
	);

	static Shader shader4 = Shader(
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 uvCoord;\n"
		"\n"
		"out vec2 v_uvCoord;"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position, 0, 1);\n"
		"	v_uvCoord = uvCoord;\n"
		"};\n"
		,
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"in vec2 v_uvCoord;"
		"uniform sampler2D u_texture;"
		"\n"
		"void main()\n"
		"{\n"
		"	color = texture(u_texture, v_uvCoord);\n"
		"	color[0] = float(int(color[0] * 8)) / 8;\n"
		"	color[1] = float(int(color[1] * 8)) / 8;\n"
		"	color[2] = float(int(color[2] * 8)) / 8;\n"
		"};\n"
	);

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_0:
			window.setPostProcessingShaderDefault();
			break;
		case GLFW_KEY_1:
			window.setPostProcessingShader(shader1);
			break;
		case GLFW_KEY_2:
			window.setPostProcessingShader(shader2);
			break;
		case GLFW_KEY_3:
			window.setPostProcessingShader(shader3);
			break;
		case GLFW_KEY_4:
			window.setPostProcessingShader(shader4);
			break;
		}
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}
