#pragma once
#include "engine/Prop.h"

class Enemy : public Prop
{

	Enemy(float x, float y, float width, float height, Texture& tex, const std::string& typeName, float z);

};