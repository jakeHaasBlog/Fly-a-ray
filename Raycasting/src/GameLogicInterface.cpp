
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

void GameLogicInterface::update(float deltaTime) {
	static float n = 0.0f;
	n += 0.01f;

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

	mysound.setPosition(YSE::Vec(sin(n) * 10, 0.0f, cos(n) * 10));
	//mysound.setVolume(1.0f - ((sin(n) * 0.5f) + 0.5f) + 0.1f);
	YSE::System().update();
}

void GameLogicInterface::cleanup() {
	tex1.freeMemory();
	tex2.freeMemory();
}