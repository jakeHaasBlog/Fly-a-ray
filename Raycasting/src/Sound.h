#pragma once

#include <string>
#include "yse.hpp"

#include <array>

class Sound {
public:
	Sound(const std::string& filepath);

	void setVolume(float volume);
	void setPlaybackSpeed(float speed);

	void play2D();
	void play3D(float x, float y, float z);
	void play3D(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos);

	const std::string& getFilepath();

private:
	YSE::sound sound;
	bool isInitialized = false;
	const std::string filepath;

	float volume = 1.0f;
	float playbackSpeed = 1.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	bool is3D = false;

	void makeSound();

};