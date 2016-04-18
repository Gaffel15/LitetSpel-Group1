#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::Initialize()
{
}

void Player::Release()
{
}

void Player::Update(double deltaTime)
{

}

void Player::Render()
{

}

void Player::Move(MovementDirection dir, double deltaTime)
{
	if (dir == UP)
	{
		position.z += position.z + 1 * (float)deltaTime;
	}
	else if (dir == DOWN)
	{
		position.z += position.z - 1 * (float)deltaTime;
	}
	else if (dir == LEFT)
	{
		position.x += position.x + 1 * (float)deltaTime;
	}
	else if (dir == RIGHT)
	{
		position.x += position.x - 1 * (float)deltaTime;
	}
}