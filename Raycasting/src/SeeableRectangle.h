#pragma once

#include "SeeableEntity.h"

class SeeableRectangle : public SeeableEntity {
public:
	SeeableRectangle(float x, float y, float width, float height, Texture* tex = nullptr);

	bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, std::array<float, 2>* poi = nullptr) override;

	virtual void renderPrimitive() override;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) override;

private:
	Geo::Rectangle rect;

};