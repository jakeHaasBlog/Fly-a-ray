#pragma once

#include "yse.hpp"
#include <string>
#include <array>

class LoopingSound {
public:
	LoopingSound(const std::string filepath);

	void setVolume(float volume);
	void setPlaybackSpeed(float speed);

	void play2D();
	void play3D();
	void setPosition(float x, float y, float z);
	void setPositionRelitive(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos);

	void pause();
	void resume();

	bool isPlaying();

	void setTime(float time);
	float getCurrentTime();
	float getMaxTime();

	float getVolume();
	float getPlaybackSpeed();

	const std::string& getFilepath();

private:
	const std::string filepath;
	bool isInitialized = false;
	float x, y, z;

	YSE::sound sound;

};

