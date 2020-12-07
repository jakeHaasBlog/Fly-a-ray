#include "Sound.h"

Sound::Sound(const std::string & filepath) :
	filepath(filepath),
	sounds(std::list<std::unique_ptr<YSE::sound>>())
{
}

Sound::~Sound()
{
}

void Sound::setVolume(float volume)
{
	this->volume = volume;
}

void Sound::setPlaybackSpeed(float speed)
{
	this->playbackSpeed = speed;
}

void Sound::play2D()
{	
	is3D = false;
	makeSound();
}

void Sound::play3D(float x, float y, float z)
{
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

	for (auto i = sounds.begin(); i != sounds.end(); i++) {
		if (!((*i)->isPlaying())) {
			sounds.erase(i);
			i--;
		}
	}
	
	sounds.emplace_back(std::make_unique<YSE::sound>());
	std::list<std::unique_ptr<YSE::sound>>::iterator sound = sounds.end();
	sound--;

	(*sound)->create(filepath.c_str());
	(*sound)->setVolume(volume);
	(*sound)->setSpeed(playbackSpeed);
	
	if (is3D) {
		(*sound)->setPosition({ x, y, z });
		(*sound)->set2D(false);
	}
	else {
		(*sound)->set2D(true);
	}

	(*sound)->play();

}
