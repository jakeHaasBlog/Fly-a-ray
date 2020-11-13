
#include "GameLogicInterface.h"

YSE::sound mysound;
Texture tex1;
Texture tex2;
TexturedQuad tq = TexturedQuad(-0.9, -0.9, 1.8, 1.8, tex1);

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

	mysound.setPosition(YSE::Vec(sin(n) * 10, 0.0f, cos(n) * 10));
	YSE::System().update();
}

void GameLogicInterface::cleanup() {
	tex1.freeMemory();
	tex2.freeMemory();
}