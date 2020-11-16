#include "SeeableEntity.h"

SeeableEntity::SeeableEntity(float x1, float y1, float x2, float y2) {
	line = Geo::LineSeg(x1, y1, x2, y2);
}

SeeableEntity::SeeableEntity(float x1, float y1, float x2, float y2, Texture * tex) {
	line = Geo::LineSeg(x1, y1, x2, y2);
	this->tex = tex;
}

bool SeeableEntity::seenBy(Geo::LineSeg& ray, float & dist, float & intersectedAt, std::array<float, 2>* pointOfIntersection) {
	std::vector<std::array<float, 2>> poi;
	if (line.getIntersection(ray, &poi)) {

		if (pointOfIntersection)
			*pointOfIntersection = poi[0];

		float x = poi[0][0];
		float y = poi[0][1];
		dist = sqrt((x - ray.x1) * (x - ray.x1) + (y - ray.y1) * (y - ray.y1));

		float dx = line.x2 - line.x1;
		float dy = line.y2 - line.y1;

		if (abs(dx) > 0.0f) intersectedAt = (poi[0][0] - line.x1) / dx;
		else intersectedAt = (poi[0][1] - line.y1) / dy;

		return true;
	}
	else {
		return false;
	}

}

void SeeableEntity::renderPrimitive() {
	line.render(color[0], color[1], color[2]);
}

void SeeableEntity::renderPrimitive(std::array<float, 2> translation, float scale) {
	Geo::LineSeg lineCopy = line;
	lineCopy.x1 *= scale;
	lineCopy.x2 *= scale;
	lineCopy.y1 *= scale;
	lineCopy.y2 *= scale;
	lineCopy.x1 += translation[0];
	lineCopy.x2 += translation[0];
	lineCopy.y1 += translation[1];
	lineCopy.y2 += translation[1];
	lineCopy.render(color[0], color[1], color[2]);
}

Texture * SeeableEntity::getTexture() {
	return tex;
}

float SeeableEntity::getR() {
	return color[0];
}

float SeeableEntity::getG() {
	return color[1];
}

float SeeableEntity::getB() {
	return color[2];
}

float SeeableEntity::getX1() {
	return line.x1;
}

float SeeableEntity::getY1(){
	return line.y1;
}

float SeeableEntity::getX2() {
	return line.x2;
}

float SeeableEntity::getY2() {
	return line.y2;
}
