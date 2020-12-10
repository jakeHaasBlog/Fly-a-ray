#include "Camera.h"

#include "engine/TexturedQuad.h"
#include "engine/Window.h"
#include <math.h>

Camera::Camera(float x, float y, float direction, float fov, int rayCount) {
	this->x = x;
	this->y = y;
	this->direction = direction;
	this->fov = fov;
	this->rayCount = rayCount;
}

void Camera::renderPrimitiveRays(std::array<float, 2> translation, float scale) {

	for (int i = 0; i < rayCount; i++) {
		float a = direction - fov/2 + (fov / rayCount) * i;
		float dirX = cos(a);
		float dirY = sin(a);

		Geo::LineSeg::renderLine(x * scale + translation[0], y * scale + translation[1], (x + dirX * 10) * scale + translation[0], (y + dirY * 10) * scale + translation[1], 1, 1, 1);
	}

}

void Camera::renderPrimitiveRays(std::array<float, 2> translation, float scale, std::vector<SeeableEntity*>& seeableEntities) {

	for (int i = 0; i < rayCount; i++) {
		float a = direction - fov / 2 + (fov / rayCount) * i;
		float dirX = cos(a);
		float dirY = sin(a);

		Geo::LineSeg ray = Geo::LineSeg(x, y, x + dirX * 10, y + dirY * 10);
		
		if (seeableEntities.size() > 0) {
			float tmp;
			std::array<float, 2> intersect;
			std::array<float, 2> closestIntersect;
			float closest;
			float dist;
			bool isIntersecting = seeableEntities[0]->seenBy(ray, dist, tmp, &intersect);
			closest = dist;
			closestIntersect = intersect;

			for (SeeableEntity* e : seeableEntities) {
				if (e->seenBy(ray, dist, tmp, &intersect)) {
					if (dist < closest || !isIntersecting) {
						closest = dist;
						closestIntersect = intersect;
					}
					isIntersecting = true;
				}
			}

			if (isIntersecting) {
				Geo::LineSeg::renderLine(x * scale + translation[0], y * scale + translation[1], closestIntersect[0] * scale + translation[0], closestIntersect[1] * scale + translation[1], 1.0f - (closest / 4.0f), 1.0f - (closest / 4.0f), 1.0f);
				Geo::Circle::fillCircle(closestIntersect[0] * scale + translation[0], closestIntersect[1] * scale + translation[1], 0.01f, 1, 0, 0);
			}
			else 
				Geo::LineSeg::renderLine(x * scale + translation[0], y * scale + translation[1], (x + dirX * 10) * scale + translation[0], (y + dirY * 10) * scale + translation[1], 1, 1, 1);
			
		}
		else {
			Geo::LineSeg::renderLine(x * scale + translation[0], y * scale + translation[1], (x + dirX * 10) * scale + translation[0], (y + dirY * 10) * scale + translation[1], 1, 1, 1);
		}

	}

}

void Camera::renderView(std::vector<SeeableEntity*>& seeableEntities) {

	for (int i = 0; i < rayCount; i++) {
		float a = direction - fov / 2 + (fov / rayCount) * i;
		float dirX = cos(a);
		float dirY = sin(a);

		float renderAreaX = ((float)(rayCount - i) / rayCount) * (window.getRightScreenBound() - window.getLeftScreenBound()) + window.getLeftScreenBound();
		float renderAreaY = 0.0f;
		float renderAreaWidth = (window.getRightScreenBound() - window.getLeftScreenBound()) / rayCount;

		Geo::LineSeg ray = Geo::LineSeg(x, y, x + dirX * 10, y + dirY * 10);

		if (seeableEntities.size() > 0) {
			float tmp;
			std::array<float, 2> intersect;
			std::array<float, 2> closestIntersect;
			float closest;
			float dist;
			SeeableEntity* entity = nullptr;
			bool isIntersecting = seeableEntities[0]->seenBy(ray, dist, tmp, &intersect);
			if (isIntersecting) entity = seeableEntities[0];
			closest = dist;
			closestIntersect = intersect;

			for (SeeableEntity* e : seeableEntities) {
				if (e->seenBy(ray, dist, tmp, &intersect)) {
					if (dist < closest || !isIntersecting) {
						closest = dist;
						closestIntersect = intersect;
						entity = e;
					}
					isIntersecting = true;
				}
			}

			if (isIntersecting) {
				float dFacing = abs(a - direction);
				float rayDist = closest * cos(dFacing);
				float height = 2.0f - rayDist / 2.0f;
				float colorFade = (height / 3.0f);
				if (height < 0) height = 0;
				Geo::Rectangle::fillRect(renderAreaX, renderAreaY - height / 2, renderAreaWidth, height, colorFade * entity->getR(), colorFade * entity->getG(), colorFade * entity->getB());
			}
		}

	}
}

void Camera::setX(float x) {
	this->x = x;
}

void Camera::setY(float y) {
	this->y = y;
}

void Camera::setDirection(float direction) {
	this->direction = direction;
}

float Camera::getX() {
	return x;
}

float Camera::getY() {
	return y;
}

float Camera::getDirection() {
	return direction;
}
