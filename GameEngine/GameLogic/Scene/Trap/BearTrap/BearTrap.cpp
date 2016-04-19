#include "BearTrap.h"



BearTrap::BearTrap()
{
}


BearTrap::~BearTrap()
{
}

void BearTrap::Initialize()
{
	slow = 3.5f; 
}

void BearTrap::Release()
{

}

void BearTrap::Update(double deltaTime)
{
	if (slow > 0.0f)
	{
		slow = slow - 1;
	}
}

void BearTrap::Render()
{

}

float BearTrap::GetSlow()
{
	return slow;
}

void BearTrap::SetSlow(float slow)
{
	this->slow = slow;
}
