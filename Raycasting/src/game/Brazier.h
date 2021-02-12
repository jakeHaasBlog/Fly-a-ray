#pragma once
#include "engine/Prop.h"

class Brazier : public Prop {
public:
	Brazier(float x, float y, float width, float height, AnimatedSprite& animatedSprite);
	void Update(float deltaTime);
};