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

	void bind() const;
	void unbind() const;

	GLuint getID() const;

	void freeMemory();

private:
	GLuint id = 0;

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

	unsigned int getCount() const;

	void bind() const;
	void unbind() const;

	GLuint getID() const;

	void freeMemory();

private:
	GLuint id = 0;
	unsigned int indexCount = 0;
};


class VertexArray {
public:

	VertexArray(const std::string& vertexDataFormat, const VertexBuffer& vb, const IndexBuffer& ib);
	VertexArray(GLuint exisingBufferID);
	VertexArray();

	void setAttributes(const std::string& vertexDataFormat, const VertexBuffer& vb, const IndexBuffer& ib);

	void bind() const;
	void unbind() const;

	GLuint getID() const;

	void freeMemory();

private:
	GLuint id;

};