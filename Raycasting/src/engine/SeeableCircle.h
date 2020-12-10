#pragma once

#include "engine/SeeableEntity.h"

class SeeableCircle : public SeeableEntity {
public:

	SeeableCircle(float x, float y, float radius);

	virtual bool seenBy(Geo::LineSeg & ray, float & dist, float & intersectedAt, std::array<float, 2>* poi = nullptr) override;
	virtual void renderPrimitive() override;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) override;

private:
	Geo::Circle circle;

};