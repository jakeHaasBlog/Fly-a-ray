#include "BatchQuads.h"

Shader* BatchQuads::defaultShader = nullptr;

BatchQuads::BatchQuads()
{
}

BatchQuads::BatchQuads(int numOfQuads) :
	vertices(numOfQuads)
{
}

BatchQuads::~BatchQuads()
{
	delete vb;
	delete ib;
	delete va;
}

void BatchQuads::addQuad(float x, float y, float width, float height, float r, float g, float b, int _texture)
{
	float texture = (float)_texture;
	// bl, tl, tr, br
	vertices.push_back({
		x         , y,          0.0f, 0.0f, r, g, b, texture,
		x         , y + height, 0.0f, 1.0f, r, g, b, texture,
		x + width , y + height, 1.0f, 1.0f, r, g, b, texture,
		x + width , y,          1.0f, 0.0f, r, g, b, texture,
	});
	buffersReady = false;
}

void BatchQuads::removeQuad(int i)
{
	vertices.erase(vertices.begin() + i);
	buffersReady = false;
}

BatchQuad & BatchQuads::getQuad(int i)
{
	buffersReady = false;
	return vertices[i];
}

void BatchQuads::setQuad(int i, float x, float y, float width, float height, float r, float g, float b)
{
	vertices[i] = {
		x         , y,          0.0f, 0.0f, r, g, b, 0,
		x         , y + height, 0.0f, 1.0f, r, g, b, 0,
		x + width , y + height, 1.0f, 1.0f, r, g, b, 0,
		x + width , y,          1.0f, 0.0f, r, g, b, 0,
	};

	buffersReady = false;
}

void BatchQuads::setQuadPos(int i, float x, float y, float width, float height)
{
	float r = vertices[i].r1;
	float g = vertices[i].g1;
	float b = vertices[i].b1;
	float tex = vertices[i].texture1;
	vertices[i] = {
		x         , y,          0.0f, 0.0f, r, g, b, tex,
		x         , y + height, 0.0f, 1.0f, r, g, b, tex,
		x + width , y + height, 1.0f, 1.0f, r, g, b, tex,
		x + width , y,          1.0f, 0.0f, r, g, b, tex,
	};

	buffersReady = false;
}

void BatchQuads::setQuadColor(int i, float r, float g, float b)
{
	vertices[i].r1 = r;
	vertices[i].r2 = r;
	vertices[i].r3 = r;
	vertices[i].r4 = r;

	vertices[i].g1 = g;
	vertices[i].g2 = g;
	vertices[i].g3 = g;
	vertices[i].g4 = g;

	vertices[i].b1 = b;
	vertices[i].b2 = b;
	vertices[i].b3 = b;
	vertices[i].b4 = b;

	buffersReady = false;
}

void BatchQuads::setTopLeftQuadColor(int i, float r, float g, float b) 
{
	vertices[i].r2 = r;
	vertices[i].g2 = g;
	vertices[i].b2 = b;

	buffersReady = false;
}

void BatchQuads::setTopRightQuadColor(int i, float r, float g, float b) 
{
	vertices[i].r3 = r;
	vertices[i].g3 = g;
	vertices[i].b3 = b;

	buffersReady = false;
}

void BatchQuads::setBottomLeftQuadColor(int i, float r, float g, float b)
{
	vertices[i].r1 = r;
	vertices[i].g1 = g;
	vertices[i].b1 = b;

	buffersReady = false;
}

void BatchQuads::setBottomRightQuadColor(int i, float r, float g, float b)
{
	vertices[i].r4 = r;
	vertices[i].g4 = g;
	vertices[i].b4 = b;

	buffersReady = false;
}

void BatchQuads::setTextureSlot(int slot, Texture * tex)
{
	boundTextures[slot] = tex;
}

void BatchQuads::useShader(Shader & shader)
{
	quadShader = &shader;
}

void BatchQuads::renderAll()
{
	if (!buffersReady) {
		updateBuffers();
		buffersReady = true;
	}

	bindTextures();

	quadShader->bind();
	va->bind();
	glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);
	quadShader->unbind();
	va->unbind();
}

int BatchQuads::size()
{
	return vertices.size();
}

void BatchQuads::updateBuffers()
{
	if (!defaultShader) {
		std::string vertexShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec2 position;\n"
			"layout(location = 1) in vec2 uvCoord;\n"
			"layout(location = 2) in vec3 color;\n"
			"layout(location = 3) in float tex\n;"
			"\n"
			"out vec2 v_uvCoord;\n"
			"out vec3 v_color;\n"
			"out float v_tex;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vec4(position, 0, 1);\n"
			"	gl_Position[2] = 0;\n"
			"   v_uvCoord = uvCoord;\n"
			"   v_color = color;\n"
			"   v_tex = tex;\n"
			"};\n";

		std::string fragmentShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 color;\n"
			"\n"
			"in vec2 v_uvCoord;\n"
			"in vec3 v_color;\n"
			"in float v_tex;\n"
			"\n"
			"uniform sampler2D u_textures[15];\n"
			"\n"
			"void main()\n"
			"{\n"
			"	int t = int(v_tex);\n"
			"	color = vec4(v_color, 1);\n"
			"   if (t != -1)\n"
			"      color = texture(u_textures[t], v_uvCoord); \n"
			"};\n";

		defaultShader = new Shader(vertexShader, fragmentShader);
	}

	if (!quadShader) {
		quadShader = defaultShader;
	}

	std::vector<unsigned int> indices(vertices.size() * 6);
	for (int i = 0; i < vertices.size(); i++) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
								 
		indices[i * 6 + 3] = i * 4 + 0;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	if (buffersInitialized) {
		vb->bufferSubData(&vertices[0], vertices.size() * sizeof(BatchQuad));
		ib->bufferSubData(&indices[0], indices.size() * sizeof(unsigned int));
	}
	else {
		ib = new IndexBuffer();
		ib->bufferData(&indices[0], indices.size() * sizeof(unsigned int));

		vb = new VertexBuffer();
		vb->bufferData(&vertices[0], vertices.size() * sizeof(BatchQuad));

		va = new VertexArray();
		va->setAttributes("ff ff fff f", *vb, *ib);

		buffersInitialized = true;
	}
}

void BatchQuads::bindTextures()
{
	for (int i = 0; i < boundTextures.size(); i++) {
		if (boundTextures[i]) {
			boundTextures[i]->bind(i);
		}
		else {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		quadShader->setUniform1i("u_textures[" + std::to_string(i) + "]", i);
	}

}
