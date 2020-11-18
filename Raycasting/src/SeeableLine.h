#pragma once

#include "SeeableEntity.h"

class SeeableLine : public SeeableEntity {
public:
	SeeableLine(float x1, float y1, float x2, float y2, Texture* tex = nullptr);

	bool seenBy(Geo::LineSeg& ray, float& dist, float& intersectedAt, std::array<float, 2>* poi = nullptr) override;

	virtual void renderPrimitive() override;
	virtual void renderPrimitive(std::array<float, 2> translation, float scale) override;

private:
	Geo::LineSeg line;

};