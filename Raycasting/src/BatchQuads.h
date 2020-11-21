#pragma once

#include <vector>

#include "OpenglBufferObjects.h"

struct BatchQuad {
	float x1, y1;
	float texX1, texY1;
	float r1, g1, b1;

	float x2, y2;
	float texX2, texY2;
	float r2, g2, b2;

	float x3, y3;
	float texX3, texY3;
	float r3, g3, b3;

	float x4, y4;
	float texX4, texY4;
	float r4, g4, b4;
};

class BatchQuads {
public:
	BatchQuads();
	BatchQuads(int numOfQuads);

	void addQuad(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void removeQuad(int i);
	BatchQuad& getQuad(int i); // use this to modify quads, NEVER create a pointer to a quad

	void renderAll();

	int size();

private:
	std::vector<BatchQuad> vertices;
	IndexBuffer ib;
	VertexBuffer vb;
	VertexArray va;
	void updateBuffers();
};