#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

class VertexBuffer {
public:
	// generate a new empty vertex buffer
	VertexBuffer();

	// store an already initialized vertex buffer
	VertexBuffer(GLuint existingBuffer);

	// generate a new vertex buffer filled with data
	VertexBuffer(void* data, size_t dataSizeInBytes);

	void bufferData(void* data, size_t dataSizeInBytes);
	void bufferSubData(void* data, size_t dataSizeInBytes);

	void bind();
	void unbind();

	GLuint getID();

	void freeMemory();

private:
	GLuint id = 0;
	bool isInitialized = false;

};


class IndexBuffer {
public:
	// generate a new empty index buffer
	IndexBuffer();

	// store an already initialized index buffer
	IndexBuffer(GLuint existingBuffer);

	// generate a new index buffer filled with data
	IndexBuffer(void* data, size_t dataSizeInBytes);

	void bufferData(void* data, size_t dataSizeInBytes);
	void bufferSubData(void* data, size_t dataSizeInBytes);

	unsigned int getCount();

	void bind();
	void unbind();

	GLuint getID();

	void freeMemory();

private:
	GLuint id = 0;
	unsigned int indexCount = 0;
	bool isInitialized = false;
};


class VertexArray {
public:

	VertexArray(const std::string& vertexDataFormat, VertexBuffer& vb, IndexBuffer& ib);
	VertexArray(GLuint exisingBufferID);
	VertexArray();

	void setAttributes(const std::string& vertexDataFormat, VertexBuffer& vb, IndexBuffer& ib);

	void bind();
	void unbind();

	GLuint getID();

	void freeMemory();

private:
	GLuint id;
	bool isInitialized = false;

};