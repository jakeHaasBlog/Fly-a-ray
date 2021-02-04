#pragma once

#include "engine/Texture.h"
#include "engine/AnimatedSprite.h"
#include <string>

/* A prop is a front-facing texture that represents any visible non-wall object
	- They can be an image, solid color, or animation
	- They have an x, y, and z coordinate
	- They have a width and height
	- Must be grouped with other props then passed into the renderProps method of a Camera to be rendered
*/
class Prop {
public:

	// constructor for texture variant
	Prop(float x, float y, float width, float height, Texture& tex, const std::string& typeName = "default", float z = 0.0f);

	// constructor for solid color variant
	Prop(float x, float y, float width, float height, std::array<float, 4>& color, const std::string& typeName = "default", float z = 0.0f);

	// constructor for animated variant
	Prop(float x, float y, float width, float height, AnimatedSprite& animatedSprite, const std::string& typeName = "default", float z = 0.0f);

	void setTypeName(const std::string& typeName); // optional: descripe what this thing is, for debugging or referencing i.e. if (getTypeName() == "powerup") { collect(); }
	const std::string& getTypeName();

	void setWidth(float width);
	float getWidth();

	void setHeight(float height); // 1 is wall height
	float getHeight();
		
	void setX(float x);
	float getX();

	void setY(float y);
	float getY();

	void setZ(float z); // 0 is floor level, 1 is wall height
	float getZ();

	void bindTexture(Texture& tex);
	Texture* getTexture(); // will return null if not a texture variant

	void bindColor(std::array<float, 4>& color);
	std::array<float, 4>* getColor(); // will return null if not a texture variant

	void bindAnimatedSprite(AnimatedSprite& sprite);
	AnimatedSprite* getAnimatedSprite(); // will return null if not an animated variant

private:
	std::string typeName;
	float width, height;
	float x, y, z;

	Texture* texture = nullptr;
	std::array<float, 4>* color = nullptr;
	AnimatedSprite* animatedSprite = nullptr;
};