#pragma once

#include <vector>
#include <array>

#include <Texture.h>

#include "OpenglBufferObjects.h"
#include "Shader.h"

#include <memory>

struct BatchQuad {
	float x1, y1;
	float texX1, texY1;
	float r1, g1, b1;
	float texture1;

	float x2, y2;
	float texX2, texY2;
	float r2, g2, b2;
	float texture2;

	float x3, y3;
	float texX3, texY3;
	float r3, g3, b3;
	float texture3;

	float x4, y4;
	float texX4, texY4;
	float r4, g4, b4;
	float texture4;
};

class BatchQuads {
public:
	BatchQuads();
	BatchQuads(int numOfQuads);

	~BatchQuads();

	void addQuad(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, int texture = 0);
	void removeQuad(int i);
	BatchQuad& getQuad(int i); // use this to modify quads, NEVER create a pointer to a quad

	void setQuad(int i, float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void setQuadPos(int i, float x, float y, float width, float height);
	void setQuadColor(int i, float r, float g, float b);

	void setTopLeftQuadColor(int i, float r, float g, float b);
	void setTopRightQuadColor(int i, float r, float g, float b);
	void setBottomLeftQuadColor(int i, float r, float g, float b);
	void setBottomRightQuadColor(int i, float r, float g, float b);

	void setTextureSlot(int slot, Texture* tex);

	void useShader(Shader& shader);

	void renderAll();

	int size();

private:
	std::vector<BatchQuad> vertices;
	IndexBuffer* ib;
	VertexBuffer* vb;
	VertexArray* va;
	void updateBuffers();
	void bindTextures();

	static Shader* defaultShader;
	Shader* quadShader;

	std::array<Texture*, 15> boundTextures;

	bool buffersReady = false;
	bool buffersInitialized = false;
};