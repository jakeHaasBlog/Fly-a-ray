#include "LoopingSound.h"

LoopingSound::LoopingSound(const std::string filepath) :
	filepath(filepath)
{
}

void LoopingSound::setVolume(float volume)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.setVolume(volume);
}

void LoopingSound::setPlaybackSpeed(float speed)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.setSpeed(speed);
}

void LoopingSound::play2D()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.set2D(true);
	sound.setTime(0);
	sound.play();
}

void LoopingSound::setPosition(float x, float y, float z)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	this->x = x;
	this->y = y;
	this->z = z;

	sound.setPosition({ x, y, z });
}

void LoopingSound::setPositionRelitive(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos)
{
	observerFacingDirection += 3.14159f / 2.0f;

	float observerFacingDirectionX = cos(observerFacingDirection);
	float observerFacingDirectionY = sin(observerFacingDirection);

	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	std::array<float, 2> vecToSound = {
		soundPos[0] - observerPos[0],
		soundPos[1] - observerPos[1],
	};

	float dotProduct = vecToSound[0] * observerFacingDirectionX + vecToSound[1] * observerFacingDirectionY;
	float magA = sqrt(pow(vecToSound[0], 2) + pow(vecToSound[1], 2));
	float magB = sqrt(pow(observerFacingDirectionX, 2) + pow(observerFacingDirectionY, 2));
	float angle = acos(dotProduct / (magA * magB));


	x = -cos(angle) * magA;
	y = 0.0f;
	z = -sin(angle) * magA;

	sound.setPosition({ x, y, z });
}

void LoopingSound::play3D()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.set2D(false);
	sound.setPosition({ x, y, z });
	sound.setTime(0);
	sound.play();
}

void LoopingSound::pause()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.pause();
}

void LoopingSound::resume()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.play();
}

void LoopingSound::setTime(float time)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	sound.setTime(time);
}

bool LoopingSound::isPlaying()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	return sound.isPlaying();
}

float LoopingSound::getCurrentTime()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	return sound.getTime();
}

float LoopingSound::getMaxTime()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	return sound.getLength();
}

float LoopingSound::getVolume()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	return sound.getVolume();
}

float LoopingSound::getPlaybackSpeed()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.setLooping(true);
		isInitialized = true;
	}

	return sound.getSpeed();
}

const std::string & LoopingSound::getFilepath()
{
	return filepath;
}
