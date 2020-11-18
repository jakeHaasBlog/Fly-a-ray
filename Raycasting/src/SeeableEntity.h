#pragma once

#include "Geometry.h"
#include "Texture.h"

class SeeableEntity {
public:

	virtual bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, std::array<float, 2>* poi = nullptr) = 0;

	virtual void renderPrimitive() = 0;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) = 0;

	Texture* getTexture();
	float getR();
	float getG();
	float getB();

protected:
	std::array<float, 3> color = { 0.8, 0.8, 0.9 };
	Texture* tex = nullptr;

};