#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>

// a texture managed by OpenGL, pixel data is stored in VRAM and accessed via shader programs
class Texture {
public:
	// default constructor, no texture will be generated and no opengl functions will be called, use this for defining Textures before GLEW is initilized
	// must call one of the generateXXX functions before binding
	Texture();

	// generates a default texture with float rbg encoding 
	Texture(int width, int height);

	// generates a texture with given data in float rgb encoding
	// pixelCount - the number of pixels in 'data'    aka. sizeof(data) / (3 * sizeof(float))
	Texture(int width, int height, float* data, size_t pixelCount);

	// generates texture then reads pixel data from png or jpg file into it
	Texture(const std::string& filename);

	// generate functions will initialize an uninitialized texture or simply overwrite an existing one
	void generateFromFile(const std::string& filename);
	// generate functions will initialize an uninitialized texture or simply overwrite an existing one
	void generateDefaultTexture(int width, int height);
	// generate functions will initialize an uninitialized texture or simply overwrite an existing one
	void generateFromData(int width, int height, float* data, size_t pixelCount);

	void setDefaultTexParameters();

	// after calling this function all opengl render calls will render onto this texture instaid of the window
	// IMPORTANT: you MUST call the unbindForRendering method when you are done rendering
	void bindForRendering(); // not implemented yet

	// unbinds this texture for rendering and sets window back to render target
	void unbindForRendering(); // not implemented yet

	// binds texture to the given slot in the vram so shader programs can access it
	void bind(int textureSlot = 0);
	void unbind();

	// 0 = linear  *will make pixels stand out (great for pixel art)*
	// 1 = nearest *will blemd pixels to make them smoother (great for realism)*
	void setSamplingMode(int sampleMode);

	// GL_CLAMP_TO_EDGE     *sampling outside the texture will grab the pixel on the closest edge and repeat it*
	// GL_CLAMP_TO_BORDER   *sampling outside the texture will give you a black pixel*
	// GL_MIRRORED_REPEAT   *sampling outside the texture will give the same texture but mirrored*
	// GL_REPEAT            *sampling outside the texture will give the same texture (great for brick walls for example)*
	void setTextureWrap(int wrappingMethod);

	// takes advantage of move semantics so returning the vector is not attrociously innefficient
	// mipmap level will return the pixels of that level of the mipmap, level 0 is the original size, each level of mipmap is half the size of previous one
	std::vector<std::array<float, 3>> getPixels(int mipmapLevel = 0);

	GLuint getID();

	// deletes the texture from VRAM
	void freeMemory();

private:
	GLuint id;
	int width, height;
	bool isInitilized = false;
};