
#include "game/GameLogicInterface.h"

// this stops the variables declared here from becoming globaly accessable
namespace {
	Camera cam = Camera(0, 0, 0, 1.2, 300);
	std::vector<SeeableEntity*> walls = {};
	BatchQuads bq;
	BatchLines bl;
	Texture bqTex;
	Texture bqTex2;
	Texture bqTex3;
	bool mouseEnabled = false;

	//SoundBite explosionSound("assets/Blast.wav");
	//LoopingSound loopingMusic("assets/Mariah Carey.wav");
	//SoundBite typeWriterSound("assets/typewriter-2.wav");

	int noiseMakerWallIndex = 0;

	BitmapText text;
	BitmapText lotsOfText;
	std::string myText = "This is some sample text...";

	AnimatedSprite spriteAnim = AnimatedSprite("assets/spritestrip.png");

	Texture brickTexture("assets/bricks.jfif");

	static std::array<float, 4> green = { 0.3f, 1.0f, 0.3f, 0.7f };
	static std::vector<Prop*> props = {
		new Prop(0.0f, 0.0f, 0.3f, 1.0f, *TextureManager::getTexture("assets/test.png")),
		new Prop(0.0f, 0.7f, 0.15f, 1.0f, green),
		new Prop(0.7f, 0.7f, 0.15f / 5.0f, 0.2f, spriteAnim, "running person collectible", 0.0f)
	};
}

void GameLogicInterface::init() {

	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	walls.push_back(new SeeableLine(1.5f, 0.9f, 1.5f, -0.9f));
	(*(walls.end() - 1))->setColor(1.0f, 0.8f, 0.6f);
	walls.push_back(new SeeableLine(-1.5f, 0.9f, -1.5f, -0.9f));
	(*(walls.end() - 1))->setColor(1.0f, 0.8f, 0.6f);
	walls.push_back(new SeeableLine(-1.5f, 0.9f, 1.5f, 0.9f));
	(*(walls.end() - 1))->setColor(1.0f, 0.8f, 0.6f);
	walls.push_back(new SeeableLine(-1.5f, -0.9f, 1.5f, -0.9f, &brickTexture));
	(*(walls.end() - 1))->setColor(1.0f, 0.8f, 0.6f);
					
	walls.push_back(new SeeableLine(-1.5f, 0.0f, -0.75f, 0.0f, &brickTexture));
	walls.push_back(new SeeableLine(-0.75f, -0.9f, -0.75f, -0.2f, &brickTexture));

	walls.push_back(new SeeableLine(-0.55f, 0.0f, -0.15f, 0.0f, &brickTexture));
	walls.push_back(new SeeableLine(-0.15f, 0.0f, -0.15f, 0.5f, &brickTexture));
	walls.push_back(new SeeableLine(-0.15f, 0.5f, -0.25f, 0.5f, &brickTexture));
	walls.push_back(new SeeableLine(-0.25f, 0.5f, -0.25, 0.9f, &brickTexture));

	walls.push_back(new SeeableLine(0.2f, 0.9f, 0.2f, 0.2f, &brickTexture));
	walls.push_back(new SeeableLine(0.2f, 0.0f, 1.5f, 0.0f, &brickTexture));
	walls.push_back(new SeeableLine(1.0f, 0.0f, 1.0f, 0.5f, &brickTexture));

	walls.push_back(new SeeableLine(0.75f, 0.0f, 0.75f, -0.7f, &brickTexture));
	walls.push_back(new SeeableLine(0.75f, -0.5f, 1.0f, -0.5f, &brickTexture));
	walls.push_back(new SeeableLine(0.75f, -0.7f, 1.3f, -0.7f, &brickTexture));

	walls.push_back(new SeeableRectangle(0, -0.35, 0.1, 0.1));
	(*(walls.end() - 1))->setColor(1.0f, 0.0f, 0.0f);
	walls.push_back(new SeeableRectangle(-0.225f, -0.35, 0.1, 0.1));
	(*(walls.end() - 1))->setColor(0.0f, 1.0f, 0.0f);
	walls.push_back(new SeeableRectangle(-0.45f, -0.35, 0.1, 0.1));
	(*(walls.end() - 1))->setColor(0.0f, 0.0f, 1.0f);

	walls.push_back(new SeeableCircle(-0.7f, 0.35f, 0.1f));
	(*(walls.end() - 1))->setColor(1.0f, 0.0f, 0.0f);

	walls.push_back(new SeeableCircle(1.3, 0.3, 0.02f));
	noiseMakerWallIndex = walls.size() - 1;
	(*(walls.end() - 1))->setColor(1.0f, 0.0f, 1.0f);

	window.setResolution(1920, 1080);

	int xWid = 200 / 2;
	int yWid = 100 / 2;
	float d = (float)(2.0f / (yWid * 1.2));
	for (int x = 0; x < xWid / 2; x++) {
		for (int y = 0; y < yWid; y++) {
			float x1 = x * d - (d * xWid / 2);
			float y1 = y * d - (d * yWid / 2);

			bq.addQuad(x1 + 1.5f, y1, d * 0.9, d * 0.9, x1 / 2, y1 / 2, x1 * y1);

		}
	}

	bqTex.generateFromFile("assets/test.png");
	bq.setTextureSlot(0, &bqTex);

	bq.addQuad(0, 0, 0.5, 0.5, 0, 0, 0, 0);
	bq.setQuadTexture(bq.size() - 1, 0);
	bq.setQuadTextureSampleBounds(bq.size() - 1, 362, 50, 100, 100);

	bq.addQuad(0.5, 0, 0.5, 0.5, 0, 0, 0, 0);
	bq.setQuadTexture(bq.size() - 1, 0);
	bq.setQuadTextureSampleBounds(bq.size() - 1, 50, 512-150, 100, 100);

	bq.addQuad(-0.5, 0, 0.5, 0.5, 0, 0, 0, 0);
	bq.setQuadTexture(bq.size() - 1, 0);
	bq.setQuadTextureSampleBounds(bq.size() - 1, 186, 512-326, 140, 140);

	int numLines = 120;
	for (int i = 0; i < numLines; i++) {
		float x = ((float)i / numLines) * 4 - 2.0f;
		bl.addLine(x / 2 - 1.0f, 0.9f, x / 2 - 1.0f, -0.9f);
	}

	bqTex.setSamplingMode(1);

	//loopingMusic.setVolume(0.3f);
	//loopingMusic.setPlaybackSpeed(1.0f);

	//loopingMusic.play3D();


	text.setText(myText);
	text.setColor(0, 0, 0);
	text.setPosition({ 0, 0 });
	text.setCharHeight(0.15f);

	text.setCharacterRenderCount(8);

	//typeWriterSound.addPart(160, 300);
	//typeWriterSound.addPart(390, 650, "click");
	//typeWriterSound.addPart(650, 850);
	//typeWriterSound.addPart(850, 1100);
	//typeWriterSound.set2D(true);
	//typeWriterSound.setVolume(0.2f);


	// initialize AnimatedSprite instance
	spriteAnim.addAnimation("walking");
	spriteAnim.bindAnimation("walking");

	spriteAnim.addFrame(80, 0, 0, 256, 256);
	spriteAnim.addFrame(80, 256, 0, 256, 256);
	spriteAnim.addFrame(80, 512, 0, 256, 256);
	spriteAnim.addFrame(80, 768, 0, 256, 256);
	spriteAnim.addFrame(80, 1024, 0, 256, 256);
	spriteAnim.addFrame(80, 1280, 0, 256, 256);

}

// deltaTime is the milliseconds between frames. Use this for calculating movement to avoid slowing down if there is lag 
void GameLogicInterface::update(float deltaTime) {


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

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



	ViewportManager::bindViewportNormalized(ViewportManager::getLeftViewportBound(), -1.0f, ViewportManager::getRightViewportBound() - ViewportManager::getLeftViewportBound(), 2.0f);
	cam.renderView(walls, props);
	Geo::Circle::fillCircle(ViewportManager::getRightViewportBound(), 0, 0.1f, 1, 0, 0);
	Geo::Circle::fillCircle(ViewportManager::getLeftViewportBound(), 0, 0.1f, 1, 0, 0);
	Geo::Circle::fillCircle(0, ViewportManager::getTopViewportBound(), 0.1f, 1, 0, 0);
	Geo::Circle::fillCircle(0, ViewportManager::getBottomViewportBound(), 0.1f, 1, 0, 0);
	ViewportManager::unbindViewport();

	static Texture minimapTexture(750 / 2, 500 / 2);
	minimapTexture.bindAsRenderTarget();
	Geo::Rectangle::fillRect(ViewportManager::getLeftViewportBound(), ViewportManager::getBottomViewportBound(), ViewportManager::getRightViewportBound() - ViewportManager::getLeftViewportBound(), ViewportManager::getTopViewportBound() - ViewportManager::getBottomViewportBound(), 0, 0, 0);
	cam.renderPrimitiveRays({ -cam.getX(), -cam.getY() }, 1.0f, walls, props);
	for (SeeableEntity* e : walls) {
		e->renderPrimitive({ -cam.getX(), -cam.getY() }, 1.0f);
	}
	minimapTexture.unbindAsRenderTarget();


	static TexturedQuad minimapQuad;
	minimapQuad.setTexture(minimapTexture);
	minimapQuad.setX(ViewportManager::getRightViewportBound() - 0.75f);
	minimapQuad.setY(ViewportManager::getBottomViewportBound());
	minimapQuad.setWidth(0.75f);
	minimapQuad.setHeight(0.5f);

	for (int i = 0; i < bq.size(); i++) {
		int r = rand() % 400;
		switch (r) {
		case 0:
			bq.setQuadTexture(i, 0);
			bq.setQuadTextureSampleBounds(i, 362, 50, 100, 100);
			break;
		case 1:
			bq.setQuadTexture(i, 0);
			bq.setQuadTextureSampleBounds(i, 50, 512 - 150, 100, 100);
			break;
		case 2:
			bq.setQuadTexture(i, 0);
			bq.setQuadTextureSampleBounds(i, 186, 512 - 326, 140, 140);
			break;
		case 3:
			bq.setQuadTexture(i, 0);
			bq.setQuadTextureSampleBounds(i, 0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case 4: case 5: case 6: case 7: case 8:
			bq.setQuadTexture(i, -1);
			break;
		}
	}
	static float x = 0.0f;
	x += 0.001f * deltaTime;

	static float y = 0.0f;
	y += 0.0005f * deltaTime;

	static float s = 0.0f;
	s += 0.01f;

	static float colorShift = 0.0f;
	colorShift += 0.03f * deltaTime / 16.0f;
	for (int i = 0; i < bl.size(); i++) {
		float percent = (float)i / bl.size();
		float colorRotator = colorShift + percent * 5;
		float colorRotator2 = colorShift - percent * 5;

		float r1 = sin(colorRotator / 1) / 2 + 0.5f;
		float g1 = sin(colorRotator / 2) / 2 + 0.5f;
		float b1 = sin(colorRotator / 4) / 2 + 0.5f;

		float r2 = sin(colorRotator2 / 1) / 2 + 0.5f;
		float g2 = sin(colorRotator2 / 2) / 2 + 0.5f;
		float b2 = sin(colorRotator2 / 4) / 2 + 0.5f;

		bl.setBeginLineColor(i, r1, g1, b1, 1.0f);
		bl.setEndLineColor(i, r2, g2, b2, 1.0f);

		BatchLine& line = bl.getLine(i);
		line.y1 = (sin(colorRotator) / 2 + 0.5f) * 0.3 + 0.7f;

	}

	//bq.renderAll(sin(s) / 3 + 1.0f, { sin(x)/4, sin(y)/4 });
	//bl.renderAll();

	static int tick = 0;
	tick++;
	if ((tick / 30) % 2 == 0) {
		walls[noiseMakerWallIndex]->setColor(1, 0, 0);
	}
	else {
		walls[noiseMakerWallIndex]->setColor(0, 1, 0);
	}
	//loopingMusic.setPosition({ cam.getX(), cam.getY() }, cam.getDirection(), { 1.3, 0.3 });

	minimapQuad.render();


	static int tCount = 0;
	tCount++;

	static int renderCount = 0;
	if (renderCount != (tCount / 3) % (text.getText().length() + 50)) {
		if (renderCount < text.getText().length())
			if (text.getText()[renderCount] != ' ') {
				//typeWriterSound.playRandomPart();
			}
	}
	renderCount = (tCount / 3) % (text.getText().length() + 50);

	text.setCharacterRenderCount(renderCount);
	text.render();

	float colorRotator = (float)tCount / 60;
	float r1 = sin(colorRotator / 1) / 2 + 0.5f;
	float g1 = sin(colorRotator / 2) / 2 + 0.5f;
	float b1 = sin(colorRotator / 4) / 2 + 0.5f;
	text.setColor(r1, g1, b1);
	text.setBackgroundColor((1.0f-g1) / 3, (1.0f - b1) / 3, (1.0f - r1) / 3, r1);

	static float time = 0.0f;
	time += deltaTime;

	bool runningLeft = false;
	float runningX = (fmod(time, 6000.0f) - 1500.0f);
	if (runningX > 1500) {
		runningX = 3000.0f -runningX;
		runningLeft = true;
	}
	runningX /= 1000.0f;

	spriteAnim.setFlipsHorozontal(runningLeft);

	spriteAnim.setX(runningX);
	spriteAnim.setWidth(0.3f);
	spriteAnim.setHeight(0.3f);
	spriteAnim.render(fmod(time, spriteAnim.getAnimationLength()));

	static AnimatedSprite s2 = spriteAnim;
	s2.setFlipsHorozontal(!runningLeft);
	s2.setX(-runningX);
	s2.setY(-0.3f);
	s2.render(fmod(time, spriteAnim.getAnimationLength()));

	props[props.size() - 1]->setZ(sin(time / 500.0f) / 2.0f + 0.5f);

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
