#include "SeeableEntity.h"

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