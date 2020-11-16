#pragma once

#include "Geometry.h"
#include "Texture.h"

class SeeableEntity {
public:
	SeeableEntity(float x1, float y1, float x2, float y2);
	SeeableEntity(float x1, float y1, float x2, float y2, Texture* tex);

	bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, std::array<float, 2>* poi = nullptr);

	void renderPrimitive();
	void renderPrimitive(std::array<float, 2> translation, float scale);

	Texture* getTexture();
	float getR();
	float getG();
	float getB();

	float getX1();
	float getY1();
	float getX2();
	float getY2();

private:
	std::array<float, 3> color = { 0.8, 0.8, 0.9 };
	Texture* tex = nullptr;
	Geo::LineSeg line;

};