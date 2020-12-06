#include "Sound.h"

Sound::Sound(const std::string & filepath) :
	filepath(filepath)
{
}

void Sound::setVolume(float volume)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.set2D(true);
		isInitialized = true;
	}
	sound.setVolume(volume);
}

void Sound::setPlaybackSpeed(float speed)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.set2D(true);
		isInitialized = true;
	}
	sound.setSpeed(speed);
}

void Sound::play2D()
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.set2D(true);
		isInitialized = true;
	}
	
	sound.set2D(true);
	sound.play();
}

void Sound::play3D(float x, float y, float z)
{
	if (!isInitialized) {
		sound.create(filepath.c_str());
		sound.set2D(true);
		isInitialized = true;
	}

	is3D = true;
	this->x = x;
	this->y = y;
	this->z = z;
	makeSound();
}

void Sound::play3D(std::array<float, 2> observerPos, float observerFacingDirection, std::array<float, 3> soundPos)
{
	observerFacingDirection += 3.14159f / 2.0f;

	float observerFacingDirectionX = cos(observerFacingDirection);
	float observerFacingDirectionY = sin(observerFacingDirection);

	if (!isInitialized) {
		sound.create(filepath.c_str());
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

	is3D = true;
	makeSound();
}

const std::string & Sound::getFilepath()
{
	return filepath;
}

void Sound::makeSound()
{
	sound.create(filepath.c_str());
	sound.setVolume(volume);
	sound.setSpeed(playbackSpeed);
	
	if (is3D) {
		sound.setPosition({ x, y, z });
		sound.set2D(false);
	}
	else {
		sound.set2D(true);
	}

	sound.play();
}
