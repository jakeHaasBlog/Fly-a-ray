#include "Enemy.h"


Enemy::Enemy(float x, float y, float width, float height, Texture& tex) : Prop(x, y, width, height, tex) {

}

void Enemy::Update(float deltaTime)
{
	setX(getX()-.0001*deltaTime);
}