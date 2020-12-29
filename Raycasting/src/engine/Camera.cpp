#include "Camera.h"

#include "engine/TexturedQuad.h"
#include "engine/Window.h"
#include "engine/ViewportManager.h"
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
			float intersectedAtReal;
			bool isIntersecting = seeableEntities[0]->seenBy(ray, dist, tmp, intersectedAtReal, &intersect);
			closest = dist;
			closestIntersect = intersect;

			for (SeeableEntity* e : seeableEntities) {
				if (e->seenBy(ray, dist, tmp, intersectedAtReal, &intersect)) {
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
	if (seeableEntities.size() == 0) return; // if there are no walls given then exit the function

	static TexturedQuad renderArea; // tool for drawing the strip every ray creates

	// for every ray being sent out
	for (int i = 0; i < rayCount; i++) {
		// calculate the direction of the current ray in 2D
		float rayDirection = direction - fov / 2 + (fov / rayCount) * i;

		// evenly distribute the strips to cover the whole viewport
		renderArea.setX(((float)(rayCount - i) / rayCount) * (ViewportManager::getRightViewportBound() - ViewportManager::getLeftViewportBound()) + ViewportManager::getLeftViewportBound());
		renderArea.setWidth((ViewportManager::getRightViewportBound() - ViewportManager::getLeftViewportBound()) / rayCount);

		// generate a line segment to represent the current ray
		Geo::LineSeg ray = Geo::LineSeg(x, y, x + cos(rayDirection) * 100, y + sin(rayDirection) * 100);

		// gather information about its closest intersection, including whether there even was an intersection or not
		impl::RayIntersectInfo closestIntersection = getClosestIntersection(ray, seeableEntities);

		// render the strip
		renderRayStrip(closestIntersection, rayDirection, renderArea);

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

impl::RayIntersectInfo Camera::getClosestIntersection(Geo::LineSeg& ray, std::vector<SeeableEntity*>& seeableEntities)
{
	// initalize info, this will be returned if there is no intersection
	impl::RayIntersectInfo info;
	info.hasIntersection = false;
	info.origin[0] = ray.x1;
	info.origin[1] = ray.y1;
	info.entity = nullptr;

	// temporary value to store new intersection
	impl::RayIntersectInfo tmp = info;

	// for every wall, put the information about its intersection with ray into tmp
	for (SeeableEntity* e : seeableEntities) {
		if (e->seenBy(ray, tmp.intersectDistance, tmp.intersectedAt, tmp.intersectedAtReal, &tmp.end)) {
			tmp.hasIntersection = true;
			tmp.entity = e;

			// if this is the first intersection store it
			if (!info.hasIntersection) {
				info = tmp;
			}
			else {
				// if the new intersection is closer than the last intersection store it, otherwise it is discarded
				if (tmp.intersectDistance < info.intersectDistance) {
					info = tmp;
				}
			}

		}
	}

	return info;
}

void Camera::renderRayStrip(impl::RayIntersectInfo intersectionInfo, float rayDirection, TexturedQuad& renderArea)
{
	// if there was no intersection, then exit this function
	if (!intersectionInfo.hasIntersection) return;

	// doing that wierd perspective thing you mentioned
	float adjustedDist = intersectionInfo.intersectDistance * cos(abs(direction - rayDirection));

	// calculating percieved height based on adjusted distance
	float percievedHeight = (1.0f / (adjustedDist + 0.1f)); // <---- this used to be 2.0f - log(3.0f * adjustedDist)

	// put the strip in the middle of the screen vertically and stretch it to percievedHeight
	renderArea.setY(-percievedHeight / 2);
	renderArea.setHeight(percievedHeight);

	if (intersectionInfo.entity->getTexture() != nullptr) {
		// the wall has a texture
		renderArea.setTexture(*intersectionInfo.entity->getTexture());
		renderArea.setTextureSampleArea(intersectionInfo.intersectedAt * 2, -1.0f, 0.03f, 2.0f);
		renderArea.render();
	}
	else {
		// the wall does not have a texture
		float r = intersectionInfo.entity->getR();
		float g = intersectionInfo.entity->getG();
		float b = intersectionInfo.entity->getB();
		Geo::Rectangle::fillRect(renderArea.getX(), renderArea.getY(), renderArea.getWidth(), renderArea.getHeight(), r, g, b);
	}

}
