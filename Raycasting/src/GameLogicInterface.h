#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Window.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include "Geometry.h"
#include "yse.hpp"


namespace GameLogicInterface {
	void init();
	void update(float deltaTime);
	void cleanup();
};