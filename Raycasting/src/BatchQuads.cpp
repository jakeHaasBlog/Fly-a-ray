#include "BatchQuads.h"

BatchQuads::BatchQuads()
{
}

BatchQuads::BatchQuads(int numOfQuads) :
	vertices(numOfQuads)
{
}

void BatchQuads::addQuad(float x, float y, float width, float height, float r, float g, float b)
{
	// bl, tl, tr, br
	vertices.push_back({
		x         , y,          0.0f, 0.0f, r, g, b,
		x         , y + height, 0.0f, 1.0f, r, g, b,
		x + width , y + height, 1.0f, 1.0f, r, g, b,
		x + width , y,          1.0f, 0.0f, r, g, b,
	});
}

void BatchQuads::removeQuad(int i)
{
	vertices.erase(vertices.begin() + i);
}

BatchQuad & BatchQuads::getQuad(int i)
{
	return vertices[i];
}

void BatchQuads::renderAll()
{
}

int BatchQuads::size()
{
	return vertices.size();
}
