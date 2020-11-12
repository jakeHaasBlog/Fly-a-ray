#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <array>

class Window {
public:
	static Window& getWindowInstance();

private:
	Window();

public:

	void mainUpdateLoop();

	void setSize(int width, int height);
	void setTitle(const std::string& title);

	int getWidth();
	int getHeight();
	std::string getTitle();

	float getAspectRatio();

	// based on the time between actually seeing frames, v-sync caps this to 60FPS
	double getFrameRate();

	float getLeftScreenBound();
	float getRightScreenBound();
	float getTopScreenBound();
	float getBottomScreenBound();

	GLFWwindow* getHandle();

private:
	std::string title;
	GLFWwindow* windowHandle;

	std::array<float, 4> viewportBounds; // left, right, top, bottom
	float aspectRatio = 1.0f;

	void calculateFPS(); // called every exactly 10 frames 
	float fps;

	void setViewport(); // scale window, correct aspect ratio

};

#define window Window::getWindowInstance()