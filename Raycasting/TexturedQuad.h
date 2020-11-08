#pragma once

#include <vector>
#include <vector>
#include <array>

struct QuadVertexData {
	float x, y, width, height;
	float texX, texY, texWidth, texHeight;
	unsigned int textureID;
};

class TexturedQuads {
public:
	// specify the number of quads in this unit to be initialized
	TexturedQuads(int count);


};