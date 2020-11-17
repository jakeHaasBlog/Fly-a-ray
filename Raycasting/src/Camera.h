#pragma once

#include <array>

#include "SeeableEntity.h"

class Camera {
public:
	Camera(float x, float y, float direction, float fov, int rayCount);

	void renderPrimitiveRays(std::array<float, 2> translation, float scale);
	void renderPrimitiveRays(std::array<float, 2> translation, float scale, std::vector<SeeableEntity>& seeableEntities);

	void renderView(std::vector<SeeableEntity>& seeableEntities);

	void setX(float x);
	void setY(float y);
	void setDirection(float direction);

	float getX();
	float getY();
	float getDirection();

private:
	float x, y;
	float direction;
	float fov;
	int rayCount;

};