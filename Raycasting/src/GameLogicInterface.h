#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Window.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include "Geometry.h"
#include "Camera.h"
#include "SeeableEntity.h"
#include "yse.hpp"
#include "SeeableLine.h"
#include "SeeableRectangle.h"
#include "SeeableCircle.h"
#include "BatchQuads.h"
#include "BatchLines.h"

namespace GameLogicInterface {
	void init();
	void update(float deltaTime);
	void cleanup();

	void mouseMoveCallback(double xPos, double yPos);
	void mouseButtonCallback(int button, int action, int mods);
	void keyCallback(int key, int scancode, int action, int mods);
	void characterCallback(unsigned int codepoint);
};