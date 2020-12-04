#include "Texture.h"

#include <array>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Window.h"
#include <iostream>

std::stack<std::array<int, 4>> Texture::renderTargetBoundings;
std::stack<int> Texture::renderTargetIDs;

Texture::Texture() {
	isInitilized = false;
}

Texture::Texture(GLuint id)
{
	this->id = id;
	isInitilized = true;
}

Texture::Texture(int width, int height) {
	generateDefaultTexture(width, height);
}

Texture::Texture(int width, int height, float * data, size_t pixelCount) {
	generateFromData(width, height, data, pixelCount);
}

Texture::Texture(const std::string & filename) {
	generateFromFile(filename);
}

void Texture::generateFromFile(const std::string & filename) {

	stbi_set_flip_vertically_on_load(true);

	int bitsPerPixel;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &bitsPerPixel, 4);
	std::vector<std::array<float, 4>> convertedPixels(width * height);

	for (int i = 0; i < width * height * 4; i++) {
		int pixIndex = i / 4;
		int pixPart = i % 4;

		convertedPixels[pixIndex][pixPart] = (float)data[i] / 255;
	}

	delete[] data;

	if (!isInitilized) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		setDefaultTexParameters();
		glBindTexture(GL_TEXTURE_2D, id);
		glGenerateMipmap(GL_TEXTURE_2D);
		generateFrameBuffer();
	}
	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &convertedPixels[0]);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	isInitilized = true;
}

void Texture::generateDefaultTexture(int width, int height) {
	this->width = width;
	this->height = height;
	if (!isInitilized) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		setDefaultTexParameters();
		glBindTexture(GL_TEXTURE_2D, id);
		glGenerateMipmap(GL_TEXTURE_2D);
		generateFrameBuffer();
	}
	glBindTexture(GL_TEXTURE_2D, id);
	
	std::vector<std::array<float, 4>> pixels(width * height);
	float r, g, b;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
	
			pixels[x + y * width][0] = 0.0f;
			pixels[x + y * width][1] = 1.0f;
			pixels[x + y * width][2] = 0.2f;
			pixels[x + y * width][3] = 1.0f;
	
			if ((x / 50) % 2 == 0 != (y / 50) % 2 == 1) {
				pixels[x + y * width][0] = 1.0f;
				pixels[x + y * width][1] = 0.0f;
				pixels[x + y * width][2] = 1.0f;
				pixels[x + y * width][3] = 1.0f;
			}
	
		}
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &pixels[0]);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	isInitilized = true;
}

void Texture::generateFromData(int width, int height, float * data, size_t pixelCount) {
	this->width = width;
	this->height = height;
	if (!isInitilized) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		setDefaultTexParameters();
		glBindTexture(GL_TEXTURE_2D, id);
		glGenerateMipmap(GL_TEXTURE_2D);
		generateFrameBuffer();
	}
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	isInitilized = true;
}

void Texture::setDefaultTexParameters() {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(int textureSlot) {
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindAsRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glViewport(0, 0, width, height);

	renderTargetBoundings.push({ 0, 0, width, height });
	renderTargetIDs.push(frameBufferID);
}

void Texture::unbindAsRenderTarget()
{
	if (!renderTargetIDs.empty()) {

		glBindTexture(GL_TEXTURE_2D, id);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		renderTargetBoundings.pop();
		renderTargetIDs.pop();

		if (!renderTargetIDs.empty()) {
			glBindFramebuffer(GL_FRAMEBUFFER, renderTargetIDs.top());
			glViewport(renderTargetBoundings.top()[0], renderTargetBoundings.top()[1], renderTargetBoundings.top()[2], renderTargetBoundings.top()[3]);
		}
		else {
			bindWindowAsRenderTarget();
		}
	}
	else {
		bindWindowAsRenderTarget();
	}
}

void Texture::bindWindowAsRenderTarget()
{
	glBindTexture(GL_TEXTURE_2D, id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0, 0, window.getWidth(), window.getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!renderTargetIDs.empty()) {
		renderTargetIDs.pop();
		renderTargetBoundings.pop();
	}
}

void Texture::setSamplingMode(int sampleMode) {
	if (sampleMode == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (sampleMode == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void Texture::setTextureWrap(int wrappingMethod) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMethod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMethod);
}

std::vector<std::array<float, 4>> Texture::getPixels(int mipmapLevel) {
	std::vector<std::array<float, 4>> pixels(width * height);

	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexImage(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, GL_FLOAT, &pixels[0]);
	unbind();

	return pixels;
}

GLuint Texture::getID() {
	return id;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

void Texture::freeMemory() {
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteTextures(1, &id);
	unbind();
}

void Texture::generateFrameBuffer()
{
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
