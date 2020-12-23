#include "engine/Window.h"
#include <chrono>

#include "game/GameLogicInterface.h"
#include "yse.hpp"

#include "engine/ViewportManager.h"
#include "engine/SoundBite.h"

Window & Window::getWindowInstance() {
	static Window w;
	return w;
}

Window::Window() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_MAXIMIZED, true);
	windowHandle = glfwCreateWindow(1920, 1080, "Ray Casting Project", NULL, NULL);
	glfwMakeContextCurrent(windowHandle);
	glfwSwapInterval(1);

	glfwSetKeyCallback(windowHandle, keyCallback);
	glfwSetCharCallback(windowHandle, charCallback);
	glfwSetMouseButtonCallback(windowHandle, mouseButtonCallback);
	glfwSetCursorPosCallback(windowHandle, mouseMoveCallback);
}

void Window::mainUpdateLoop() {

	std::chrono::high_resolution_clock clock;
	std::chrono::time_point<std::chrono::steady_clock> start = clock.now();
	std::chrono::time_point<std::chrono::steady_clock> end = clock.now();

	std::chrono::time_point<std::chrono::steady_clock> startUpdateTIme = clock.now();
	float updateTime = 0.0f;
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(windowHandle)) {
		glfwSwapInterval(1);
		glfwSwapBuffers(windowHandle);

		startUpdateTIme = clock.now();

		glfwPollEvents();

		end = clock.now();
		deltaTime = (float)((float)(end - start).count() / 1000000.0f);
		start = clock.now();

		//framebuffer.bindAsRenderTarget();
		GameLogicInterface::update(deltaTime);
		//framebuffer.unbindAsRenderTarget();


		//static float vertices[] = {
		//	-1.0f, -1.0f, 0.0f, 0.0f,
		//	-1.0f, 1.0f, 0.0f, 1.0f,
		//	1.0f, 1.0f, 1.0f, 1.0f,
		//	1.0f, -1.0f, 1.0f, 0.0f
		//};
		//static VertexBuffer vb = VertexBuffer(vertices, 4 * 4 * sizeof(float));
		//
		//static unsigned int indices[] = {
		//	0, 1, 2,
		//	0, 2, 3
		//};
		//static IndexBuffer ib = IndexBuffer(indices, 6 * sizeof(unsigned int));
		//
		//static VertexArray va = VertexArray("ff ff", vb, ib);
		//
		//static std::string vertexShaderString =
		//	"#version 330 core\n"
		//	"\n"
		//	"layout(location = 0) in vec2 position;\n"
		//	"layout(location = 1) in vec2 uvCoord;\n"
		//	"\n"
		//	"out vec2 v_uvCoord;"
		//	"\n"
		//	"void main()\n"
		//	"{\n"
		//	"	gl_Position = vec4(position, 0, 1);\n"
		//	"	v_uvCoord = uvCoord;\n"
		//	"};\n";
		//static std::string fragmentShaderString =
		//	"#version 330 core\n"
		//	"\n"
		//	"layout(location = 0) out vec4 color;\n"
		//	"\n"
		//	"in vec2 v_uvCoord;"
		//	"uniform sampler2D u_texture;"
		//	"\n"
		//	"void main()\n"
		//	"{\n"
		//	"	color = texture(u_texture, v_uvCoord);"
		//	"};\n";
		//static Shader sh = Shader(vertexShaderString, fragmentShaderString);
		//framebuffer.bind(1);
		//sh.setUniform1i("u_texture", 1);
		
		//ViewportManager::bindViewportPixels(0, 0, getWidth(), getHeight());
		//sh.bind();
		//va.bind();
		//glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr); // draws the framebuffer stretched over the whole screen
		//va.unbind();
		//sh.unbind();
		//ViewportManager::unbindViewport();

		updateTime = (float)(((double)(clock.now() - startUpdateTIme).count()) / 1000000.0f);

		ViewportManager::update();
		YSE::System().update();
		SoundBite::update();
		glfwMakeContextCurrent(windowHandle);

		static uint64_t frame = 0;
		frame++;
		if (frame % 10 == 0) {
			calculateFPS();

			char t[256];
			sprintf_s(t, "FPS: %.1f - delta time(%.2fms) - update time(%fms)", (float)getFrameRate(), deltaTime, updateTime);
			setTitle(t);
		}
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

int Window::getFrameBufferWidth()
{
	return framebuffer.getWidth();
}

int Window::getFrameBufferHeight()
{
	return framebuffer.getHeight();
}

float Window::getAspectRatio() {
	int width, height;
	glfwGetWindowSize(windowHandle, &width, &height);
	return (float)width / height;
}

double Window::getFrameRate(){
	return fps;
}

bool Window::keyIsDown(int glfwKey) {
	int state = glfwGetKey(windowHandle, glfwKey);
	return state == GLFW_PRESS;
}

float Window::getMouseX() {
	return mouseX;
}

float Window::getMouseY() {
	return mouseY;
}

void Window::close() {
	glfwSetWindowShouldClose(windowHandle, true);
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

void Window::setResolution(int width, int height)
{
	float* data = new float[width * height * 4];
	framebuffer.generateFromData(width, height, data, width * height);
	delete[] data;
}

void Window::keyCallback(GLFWwindow * wind, int key, int scancode, int action, int mods) {
	GameLogicInterface::keyCallback(key, scancode, action, mods);
}

void Window::charCallback(GLFWwindow * wind, unsigned int codepoint) {
	GameLogicInterface::characterCallback(codepoint);
}

void Window::mouseButtonCallback(GLFWwindow * wind, int button, int action, int mods) {
	GameLogicInterface::mouseButtonCallback(button, action, mods);
}

void Window::mouseMoveCallback(GLFWwindow * wind, double xPos, double yPos) {
	int width, height;
	glfwGetWindowSize(window.windowHandle, &width, &height);
	window.mouseX = ((float)(xPos - width / 2) / (width / 2)) * window.getAspectRatio();
	window.mouseY = (float)(-yPos + height / 2) / (height / 2);
	GameLogicInterface::mouseMoveCallback(xPos, yPos);
}
