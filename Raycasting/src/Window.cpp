#include "Window.h"
#include <chrono>

#include "GameLogicInterface.h"
#include "yse.hpp"

Window & Window::getWindowInstance() {
	static Window w;
	return w;
}

Window::Window() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	windowHandle = glfwCreateWindow(1920, 1080, "Ray Casting Project", NULL, NULL);
	glfwMakeContextCurrent(windowHandle);
	glfwSwapInterval(1);
}

void Window::mainUpdateLoop() {

	std::chrono::high_resolution_clock clock;
	std::chrono::time_point<std::chrono::steady_clock> start = clock.now();
	std::chrono::time_point<std::chrono::steady_clock> end = clock.now();
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(windowHandle)) {
		setViewport();
		glfwSwapInterval(1);
		glfwSwapBuffers(windowHandle);
		glfwPollEvents();

		end = clock.now();
		deltaTime = (float)((end - start).count() / 1000000);
		start = clock.now();
		GameLogicInterface::update(deltaTime);

		static uint64_t frame = 0;
		frame++;
		if (frame % 10 == 0) {
			calculateFPS();

			char t[50];
			sprintf_s(t, "FPS: %.1f", getFrameRate());
			setTitle(t);
		}

		YSE::System().update();
		glfwMakeContextCurrent(windowHandle);
	}

}

void Window::setSize(int width, int height) {
	glfwSetWindowSize(windowHandle, width, height);
}

void Window::setTitle(const std::string & title) {
	this->title = title;
	glfwSetWindowTitle(windowHandle, this->title.c_str());
}

int Window::getWidth() {
	int width;
	glfwGetWindowSize(windowHandle, &width, nullptr);
	return width;
}

int Window::getHeight() {
	int height;
	glfwGetWindowSize(windowHandle, nullptr, &height);
	return height;
}

std::string Window::getTitle() {
	return title;
}

float Window::getAspectRatio() {
	return aspectRatio;
}

double Window::getFrameRate(){
	return fps;
}

float Window::getLeftScreenBound() {
	return viewportBounds[0];
}

float Window::getRightScreenBound(){
	return viewportBounds[1];
}

float Window::getTopScreenBound() {
	return viewportBounds[2];
}

float Window::getBottomScreenBound() {
	return viewportBounds[3];
}

GLFWwindow * Window::getHandle() {
	return windowHandle;
}

void Window::calculateFPS() {
	static std::chrono::high_resolution_clock clock;
	static long long timeA;
	static long long timeB;
	timeA = clock.now().time_since_epoch().count();

	long long deltaTime = timeA - timeB;

	timeB = clock.now().time_since_epoch().count();

	fps =  1.0f / ((double)(deltaTime / 10) / 1000000000);
}

void Window::setViewport() {
	int width, height;
	glfwGetWindowSize(windowHandle, &width, &height);

	aspectRatio = (float)width / height;
	glViewport(0, 0, width, height);

	viewportBounds = { -1 * aspectRatio,  1 * aspectRatio, 1.0f, -1.0f};
}
