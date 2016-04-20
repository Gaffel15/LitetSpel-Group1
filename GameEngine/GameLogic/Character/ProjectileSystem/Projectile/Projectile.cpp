#include "Projectile.h"
inline DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 a, float b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;

	return result;
}

inline DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b) {
	DirectX::XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}


Projectile::Projectile()
{

	isFired = false;



}

Projectile::Projectile(DirectX::XMFLOAT3 origin,DirectX::XMFLOAT3 direction)
{
	//Initialize();


}


Projectile::~Projectile()
{

}

void Projectile::Initialize(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 direction)
{


	this->position = origin;
	this->direction = direction;

	this->age = 0.0f;
	this->speed = 10.0f;
	this->dmgMultiplier = 2.0f;

	this->isFired = true;
	this->collided = false;
	
}

void Projectile::Update(double deltatime)
{

	if(isFired == true)
	{
		age += deltatime;
		position = position+(direction*speed*deltatime);
	}
	if (age >= 2.5f)
		isFired = false;


}

void Projectile::Release()
{
	
}

void Projectile::Collision()
{

}

float Projectile::GetAge()
{
	return age;
}

float Projectile::GetSpeed()
{
	return speed;
}

DirectX::XMFLOAT3 Projectile::GetPos()
{
	return position;
}

DirectX::XMFLOAT3 Projectile::GetDir()
{
	return direction;
}

bool Projectile::GetFired()
{
	return isFired;
}

void Projectile::SetFired(bool isFired)
{
	this->isFired = isFired;
}

void Projectile::SetAge(float age)
{
	this->age = age;
}

void Projectile::SetPos(DirectX::XMFLOAT3 pos)
{
	this->position = pos;
}


