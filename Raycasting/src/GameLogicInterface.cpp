
#include "GameLogicInterface.h"

YSE::sound mysound;
Texture tex1;
Texture tex2;
TexturedQuad tq = TexturedQuad(-0.9, -0.9, 1.8, 1.8, tex1);
std::list<std::array<float, 5>> fragments;

Geo::Circle c;

void GameLogicInterface::init() {
	mysound.create("assets/ShootingStars.ogg");
	mysound.setLooping(true);
	mysound.play();

	tex1.generateFromFile("assets/meme.jpg");
	tex2.generateFromFile("assets/pattern.png");
}

// deltaTime is the milliseconds between frames. Use this for calculating movement to avoid slowing down if there is lag 
void GameLogicInterface::update(float deltaTime) {
	static float n = 0.0f;
	n += 0.001f * deltaTime;
	printf("Deltatime = %f\n", deltaTime);


	// I like to call this the suffering function...
	float lagCauser = 0.0f;
	for (int i = 0; i < 1500000 * ((sin(n / 5.0f) + 1.0f) / 2.0f); i++) { 
		lagCauser *= sin((float)i / 1000);
	}
	// purposely causes occasional lag to demonstrate how deltaTime should be used
	// note that the suffering function only works in debug mode. In release mode the compiler optimizes it away because the lagCauser variable isn't used anywhere

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if ((int)n / 5 % 2 == 0)
		tq.setTexture(tex1);
	else
		tq.setTexture(tex2);

	tq.setTextureSampleArea(n / 2, n / 3, sin(n) + 1.1f, sin(n) + 1.1f);
	tq.render();

	Geo::Circle::fillCircle(window.getLeftScreenBound(), 0.0f, 0.3f, 0.7f, 0.0f, 0.0f);
	Geo::Circle::fillCircle(window.getRightScreenBound(), 0.0f, 0.3f, 0.7f, 0.0f, 0.0f);
	
	Geo::Circle::fillCircle(0.0f, window.getTopScreenBound(), 0.3f, 0.7f, 0.0f, 0.0f);
	Geo::Circle::fillCircle(0.0f, window.getBottomScreenBound(), 0.3f, 0.7f, 0.0f, 0.0f);

	Geo::Rectangle::fillRect( 0.5f,  0.5f, 0.1f, 0.1f, 0.3f, 0.2f, 1.0f);
	Geo::Rectangle::fillRect(-0.6f,  0.5f, 0.1f, 0.1f, 0.3f, 0.2f, 1.0f);
	Geo::Rectangle::fillRect(-0.6f, -0.6f, 0.1f, 0.1f, 0.3f, 0.2f, 1.0f);
	Geo::Rectangle::fillRect( 0.5f, -0.6f, 0.1f, 0.1f, 0.3f, 0.2f, 1.0f);

	Geo::LineSeg::renderLine(-1.0f, 1.0f, 1.0f, -1.0f, 1, 1, 1);
	Geo::LineSeg::renderLine(1.0f, 1.0f, -1.0f, -1.0f, 1, 1, 1);

	static float greenCircleVelocity = 0.001f;
	static float greenCirclePos = 0.0f;

	// another example of using deltatime to create smooth motion, this is the code for the green ball that goes back and forth across the screen
	greenCirclePos += greenCircleVelocity * deltaTime;
	float renderAt = fmod(greenCirclePos, 2.0f);
	if ((int)(greenCirclePos / 2.0f) % 2 == 0)
		renderAt = 1.0f - renderAt;
	else
		renderAt = -1.0f + renderAt;
	Geo::Circle::fillCircle(renderAt, 0.0f, 0.05f, 0.1f, 1.0f, 0.3f);
	Geo::Circle::fillCircle(renderAt, 0.0f, 0.04f, 0.1f, 0.7f, 0.2f);

	c.x = window.getMouseX();
	c.y = window.getMouseY();
	c.radius = 0.05f;
	c.renderFilled(1, 0, 0);
	for (auto itter = fragments.begin(); itter != fragments.end(); itter++) {

		Geo::Circle::fillCircle((*itter)[0], (*itter)[1], 0.03f, 1.0f, 0.3f, 0.4f);

		(*itter)[0] += (*itter)[2] * deltaTime;
		(*itter)[1] += (*itter)[3] * deltaTime;
		(*itter)[4] -= deltaTime / 16.0f;

		if ((*itter)[4] <= 0.0f) {
			fragments.erase(itter);
		}
	}


	mysound.setPosition(YSE::Vec(sin(n) * 10, 0.0f, cos(n) * 10));
	YSE::System().update();
}

void GameLogicInterface::cleanup() {
	tex1.freeMemory();
	tex2.freeMemory();
}

void GameLogicInterface::mouseMoveCallback(double xPos, double yPos)
{
}

void GameLogicInterface::mouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		float offset = ((float)rand() / RAND_MAX) * 3.14159f * 2.0f;
		for (int i = 0; i < 10; i++) {
			fragments.push_back({
				window.getMouseX(),
				window.getMouseY(),
				sin(3.14159f * 2.0f * (float)i / 10 + offset) * 0.00036f,
				cos(3.14159f * 2.0f * (float)i / 10 + offset) * 0.00036f,
				600.0f
			});
		}
	}
}

void GameLogicInterface::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		for (int i = 0; i < 100; i++) {
			float offset = ((float)rand() / RAND_MAX) * 3.14159f - 3.14159f / 2;
			fragments.push_back({
				0,
				0,
				sin(offset) * 0.00036f,
				cos(offset) * 0.00036f,
				600.0f
			});
		}
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		for (int i = 0; i < 100; i++) {
			float offset = ((float)rand() / RAND_MAX) * 3.14159f - 3.14159f;
			fragments.push_back({
				0,
				0,
				sin(offset) * 0.00036f,
				cos(offset) * 0.00036f,
				600.0f
				});
		}
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		for (int i = 0; i < 100; i++) {
			float offset = ((float)rand() / RAND_MAX) * 3.14159f + 3.14159f / 2;
			fragments.push_back({
				0,
				0,
				sin(offset) * 0.00036f,
				cos(offset) * 0.00036f,
				600.0f
				});
		}
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		for (int i = 0; i < 100; i++) {
			float offset = ((float)rand() / RAND_MAX) * 3.14159f;
			fragments.push_back({
				0,
				0,
				sin(offset) * 0.00036f,
				cos(offset) * 0.00036f,
				600.0f
				});
		}
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}
