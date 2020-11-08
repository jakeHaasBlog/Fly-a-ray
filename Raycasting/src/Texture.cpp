#include "Texture.h"

#include <array>
#include <vector>

Texture::Texture(int width, int height) {

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	std::vector<std::array<float, 3>> pixels(width * height);
	float r, g, b;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			pixels[x + y * width][0] = 0.0f;
			pixels[x + y * width][1] = 1.0f;
			pixels[x + y * width][2] = 0.2f;

			if ((x / 50) % 2 == 0 ^ (y / 50) % 2 == 1) {
				pixels[x + y * width][0] = 1.0f;
				pixels[x + y * width][1] = 0.0f;
				pixels[x + y * width][2] = 1.0f;
			}

		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, &pixels[0]);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::Texture(int width, int height, float * data, size_t pixelCount) {

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);

	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::Texture(const std::string & filename) {

}

void Texture::bindForRendering() {

}

void Texture::unbindForRendering() {

}

void Texture::subData(float * data, size_t pixelCount) {
	glBufferSubData(GL_TEXTURE_2D, 0, pixelCount * 3 * sizeof(float), data);
}

void Texture::bind(int textureSlot) {
	glActiveTexture(id);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
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

std::vector<float> Texture::getPixels(int mipmapLevel) {
	return std::vector<float>();
}

GLuint Texture::getID() {
	return id;
}

void Texture::freeMemory() {
	glDeleteTextures(1, &id);
	unbind();
}
