#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "engine/Window.h"
#include "engine/Texture.h"
#include "engine/TexturedQuad.h"
#include "engine/Geometry.h"
#include "engine/Camera.h"
#include "engine/SeeableEntity.h"
#include "yse.hpp"
#include "engine/SeeableLine.h"
#include "engine/SeeableRectangle.h"
#include "engine/SeeableCircle.h"
#include "engine/BatchQuads.h"
#include "engine/BatchLines.h"
#include "engine/SoundBite.h"
#include "engine/LoopingSound.h"
#include "engine/ViewportManager.h"
#include "engine/BitmapFont.h"
#include "engine/BitmapText.h"
#include "engine/AnimatedSprite.h"
#include "engine/TextureManager.h"

namespace GameLogicInterface {
	void init();
	void update(float deltaTime);
	void cleanup();

	void mouseMoveCallback(double xPos, double yPos);
	void mouseButtonCallback(int button, int action, int mods);
	void keyCallback(int key, int scancode, int action, int mods);
	void characterCallback(unsigned int codepoint);
};