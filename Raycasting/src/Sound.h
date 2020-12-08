#pragma once

#include <string>
#include "yse.hpp"

#include <array>

#include <list>
#include <memory>

class Sound {
public:
	// volume defaults to 0.3 and is paused
	Sound(const std::string& filepath);
	~Sound();

	// between 0 and 1
	void setVolume(float volume);

	// 0.5 = half speed, 5 = 5 times speed
	void setPlaybackSpeed(float speed);

	// flat sound, not affected by position
	void play2D();

	// sets the 3D position of the sound when the observer is at 0, 0, 0, facing towards 0, 0, 1
	void play3D(float x, float y, float z);

	// calculates and sets the desired position of the sound based on the sound position relitive to a given observer position and direction
	void play3D(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos);

	const std::string& getFilepath();

private:
	bool isInitialized = false;
	const std::string filepath;

	float volume = 1.0f;
	float playbackSpeed = 1.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	bool is3D = false;

	void makeSound();

	std::list<std::unique_ptr<YSE::sound>> sounds;

};