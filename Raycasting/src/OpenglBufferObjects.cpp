#include "OpenglBufferObjects.h"

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &id);
}

VertexBuffer::VertexBuffer(GLuint existingBuffer) {
	id = existingBuffer;
}

VertexBuffer::VertexBuffer(void * data, size_t dataSizeInBytes) : VertexBuffer() {
	bufferData(data, dataSizeInBytes);
}

void VertexBuffer::bufferData(void * data, size_t dataSizeInBytes) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, dataSizeInBytes, data, GL_STATIC_DRAW);
	unbind();
}

void VertexBuffer::bufferSubData(void * data, size_t dataSizeInBytes) {
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSizeInBytes, data);
	unbind();
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint VertexBuffer::getID() const {
	return id;
}

void VertexBuffer::freeMemory() {
	glDeleteBuffers(GL_ARRAY_BUFFER, &id);
	unbind();
}




IndexBuffer::IndexBuffer() {
	glGenBuffers(1, &id);
}

IndexBuffer::IndexBuffer(GLuint existingBuffer) {
	id = existingBuffer;
}

IndexBuffer::IndexBuffer(void * data, size_t dataSizeInBytes) : IndexBuffer() {
	bufferData(data, dataSizeInBytes);
}

void IndexBuffer::bufferData(void * data, size_t dataSizeInBytes) {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSizeInBytes, data, GL_STATIC_DRAW);
	unbind();

	indexCount = dataSizeInBytes / sizeof(unsigned int);
}

void IndexBuffer::bufferSubData(void * data, size_t dataSizeInBytes) {
	bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dataSizeInBytes, data);
	unbind();

	indexCount = dataSizeInBytes / sizeof(unsigned int);
}

unsigned int IndexBuffer::getCount() {
	return indexCount;
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint IndexBuffer::getID() const {
	return id;
}

void IndexBuffer::freeMemory()
{
	glDeleteBuffers(1, &id);
	unbind();
}



VertexArray::VertexArray(const std::string & vertexDataFormat, const VertexBuffer & vb, const IndexBuffer & ib) {
	glGenVertexArrays(1, &id);
	setAttributes(vertexDataFormat, vb, ib);
}

VertexArray::VertexArray(GLuint exisingBufferID) {
	id = exisingBufferID;
}

VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
}

void VertexArray::setAttributes(const std::string & vertexDataFormat, const VertexBuffer & vb, const IndexBuffer & ib) {
	bind();
	vb.bind();
	ib.bind();

	int currentAttributeIndex = 0;
	int totalOffset = 0;
	std::string currentAttribFormat = "";
	int totalStride = 0;

	for (char c : vertexDataFormat) {
		if (c == 'f') totalStride += sizeof(float);
	}

	for (char c : vertexDataFormat) {
		if (c == ' ') {
			glEnableVertexAttribArray(currentAttributeIndex);

			if (currentAttribFormat.at(0) == 'f') {
				glVertexAttribPointer
				(
					currentAttributeIndex,
					currentAttribFormat.length(),
					GL_FLOAT,
					false,
					totalStride,
					(const void*)(totalOffset)
				);

				totalOffset += currentAttribFormat.length() * sizeof(float);
			}

			currentAttributeIndex++;
			currentAttribFormat = "";
		}
		else {
			currentAttribFormat += c;
		}
	}

	glEnableVertexAttribArray(currentAttributeIndex);

	if (currentAttribFormat.at(0) == 'f') {
		glVertexAttribPointer
		(
			currentAttributeIndex,
			currentAttribFormat.length(),
			GL_FLOAT,
			false,
			totalStride,
			(const void*)(totalOffset)
		);

		totalOffset += currentAttribFormat.length() * sizeof(float);
	}

	unbind();
	vb.unbind();
	ib.unbind();
}

void VertexArray::bind()
{
	glBindVertexArray(id);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

GLuint VertexArray::getID() {
	return id;
}

void VertexArray::freeMemory()
{
	glDeleteVertexArrays(1, &id);
	unbind();
}
