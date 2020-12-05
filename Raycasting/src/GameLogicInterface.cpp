
#include "GameLogicInterface.h"

// this stops the variables declared here from becoming globaly accessable
namespace {
	Camera cam = Camera(0, 0, 0, 1.152, 100);
	std::vector<SeeableEntity*> walls = {};
	BatchQuads bq;
	BatchLines bl;
	Texture bqTex;
	Texture bqTex2;
	Texture bqTex3;
	TexturedQuad testQuad;
	bool mouseEnabled = false;

	VertexBuffer vb;
	IndexBuffer ib;
	VertexArray va;
	Shader sh("assets/basicShader.sh");
}

void GameLogicInterface::init() {

	float vert[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	vb.bufferData(vert, 8 * sizeof(float));

	unsigned int ind[] = {
		0, 1, 2,
		0, 2, 3,
	};
	ib.bufferData(ind, 6 * sizeof(unsigned int));

	va.setAttributes("ff", vb, ib);


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

	static Texture minimapTexture(192, 108);
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

	bq.renderAll(sin(s) / 3 + 1.0f, { sin(x)/4, sin(y)/4 });
	bl.renderAll();
	testQuad.render();


	sh.bind();
	va.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
	va.unbind();
	sh.unbind();


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
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}
