
#include "GameLogicInterface.h"

// this stops the variables declared here from becoming globaly accessable
namespace {
	Camera cam = Camera(0, 0, 0, 1.152, 100);
	std::vector<SeeableEntity*> walls = {};
	BatchQuads bq;
	Texture bqTex;
	Texture bqTex2;
	Texture bqTex3;
	TexturedQuad testQuad;
	bool mouseEnabled = false;
}

void GameLogicInterface::init() {

	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	walls.push_back(new SeeableLine(1.5f, 0.9f, 1.5f, -0.9f));
	walls.push_back(new SeeableLine(-1.5f, 0.9f, -1.5f, -0.9f));
	walls.push_back(new SeeableLine(-1.5f, 0.9f, 1.5f, 0.9f));
	walls.push_back(new SeeableLine(-1.5f, -0.9f, 1.5f, -0.9f));
					
	walls.push_back(new SeeableLine(-1.5f, 0.0f, -0.75f, 0.0f));
	walls.push_back(new SeeableLine(-0.75f, -0.9f, -0.75f, -0.2f));

	walls.push_back(new SeeableLine(-0.55f, 0.0f, -0.15f, 0.0f));
	walls.push_back(new SeeableLine(-0.15f, 0.0f, -0.15f, 0.5f));
	walls.push_back(new SeeableLine(-0.15f, 0.5f, -0.25f, 0.5f));
	walls.push_back(new SeeableLine(-0.25f, 0.5f, -0.25, 0.9f));

	walls.push_back(new SeeableLine(0.2f, 0.9f, 0.2f, 0.2f));
	walls.push_back(new SeeableLine(0.2f, 0.0f, 1.5f, 0.0f));
	walls.push_back(new SeeableLine(1.0f, 0.0f, 1.0f, 0.5f));

	walls.push_back(new SeeableLine(0.75f, 0.0f, 0.75f, -0.7f));
	walls.push_back(new SeeableLine(0.75f, -0.5f, 1.0f, -0.5f));
	walls.push_back(new SeeableLine(0.75f, -0.7f, 1.3f, -0.7f));

	walls.push_back(new SeeableRectangle(0, -0.35, 0.1, 0.1));
	(*(walls.end() - 1))->setColor(1.0f, 0.0f, 0.0f);
	walls.push_back(new SeeableRectangle(-0.225f, -0.35, 0.1, 0.1));
	(*(walls.end() - 1))->setColor(0.0f, 1.0f, 0.0f);
	walls.push_back(new SeeableRectangle(-0.45f, -0.35, 0.1, 0.1));
	(*(walls.end() - 1))->setColor(0.0f, 0.0f, 1.0f);

	walls.push_back(new SeeableCircle(-0.7f, 0.35f, 0.1f));
	(*(walls.end() - 1))->setColor(1.0f, 0.0f, 0.0f);

	window.setResolution(1920, 1080);

	int xWid = 200 / 1;
	int yWid = 100 / 1;
	float d = (float)(2.0f / (yWid * 1.2));
	for (int x = 0; x < xWid; x++) {
		for (int y = 0; y < yWid; y++) {
			float x1 = x * d - (d * xWid / 2);
			float y1 = y * d - (d * yWid / 2);

			bq.addQuad(x1, y1, d * 0.9, d * 0.9, x1 / 2, y1 / 2, x1 * y1);

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

	testQuad.setBounding(-1.0f, -0.5, 0.5, 0.5);
	testQuad.setTexture(bqTex);

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

	if (window.keyIsDown(GLFW_KEY_A)) {
		float deltaX = cos(cam.getDirection() + 3.14159f / 2.0f) * 0.0001f * deltaTime;
		float deltaY = sin(cam.getDirection() + 3.14159f / 2.0f) * 0.0001f * deltaTime;
		cam.setX(cam.getX() + deltaX);
		cam.setY(cam.getY() + deltaY);
	}

	if (window.keyIsDown(GLFW_KEY_D)) {
		float deltaX = cos(cam.getDirection() - 3.14159f / 2.0f) * 0.0001f * deltaTime;
		float deltaY = sin(cam.getDirection() - 3.14159f / 2.0f) * 0.0001f * deltaTime;
		cam.setX(cam.getX() + deltaX);
		cam.setY(cam.getY() + deltaY);
	}

	if (window.keyIsDown(GLFW_KEY_S)) {
		float deltaX = cos(cam.getDirection()) * 0.0002f * deltaTime;
		float deltaY = sin(cam.getDirection()) * 0.0002f * deltaTime;
		cam.setX(cam.getX() - deltaX);
		cam.setY(cam.getY() - deltaY);
	}

	cam.renderView(walls);

	static Texture minimapTexture(1920, 1080);
	minimapTexture.bindAsRenderTarget();
	Geo::Rectangle::fillRect(window.getLeftScreenBound(), window.getBottomScreenBound(), window.getRightScreenBound() - window.getLeftScreenBound(), window.getTopScreenBound() - window.getBottomScreenBound(), 0, 0, 0);
	cam.renderPrimitiveRays({ -cam.getX(), -cam.getY() }, 1.0f, walls);
	for (SeeableEntity* e : walls) {
		e->renderPrimitive({ -cam.getX(), -cam.getY() }, 1.0f);
	}
	minimapTexture.unbindAsRenderTarget();

	static TexturedQuad minimapQuad;
	minimapQuad.setTexture(minimapTexture);
	minimapQuad.setX(window.getRightScreenBound() - 0.65f);
	minimapQuad.setY(window.getBottomScreenBound());
	minimapQuad.setWidth(0.65f);
	minimapQuad.setHeight(0.4f);


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
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			bq.setQuadTexture(i, -1);
			break;
		}
	}
	bq.renderAll();
	testQuad.render();

	minimapQuad.render();
	YSE::System().update();
}

void GameLogicInterface::cleanup() {
	for (SeeableEntity* entity : walls) {
		delete entity;
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
		//glfwSetWindowShouldClose(window.getHandle(), true);
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}
