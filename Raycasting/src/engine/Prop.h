#pragma once

#include "engine/Texture.h"
#include "engine/AnimatedSprite.h"
#include <string>

class Prop {
public:

	Prop(float x, float y, float width, float height, Texture& tex, const std::string& typeName = "default", float z = 0.0f);
	Prop(float x, float y, float width, float height, std::array<float, 4>& color, const std::string& typeName = "default", float z = 0.0f);
	Prop(float x, float y, float width, float height, AnimatedSprite& animatedSprite, const std::string& typeName = "default", float z = 0.0f);

	void setTypeName(const std::string& typeName);
	const std::string& getTypeName();

	void setWidth(float width);
	float getWidth();

	void setHeight(float height);
	float getHeight();
		
	void setX(float x);
	float getX();

	void setY(float y);
	float getY();

	void setZ(float z);
	float getZ();

	void bindTexture(Texture& tex);
	Texture* getTexture();

	void bindColor(std::array<float, 4>& color);
	std::array<float, 4>* getColor();

	void bindAnimatedSprite(AnimatedSprite& sprite);
	AnimatedSprite* getAnimatedSprite();

private:
	std::string typeName;
	float width, height;
	float x, y, z;

	Texture* texture = nullptr;
	std::array<float, 4>* color = nullptr;
	AnimatedSprite* animatedSprite = nullptr;
};