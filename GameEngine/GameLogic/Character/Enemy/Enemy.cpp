#include "Enemy.h"

Enemy::Enemy(XMFLOAT3 spawn)
{
	this->position = spawn;
	Initialize();
	this->enemyStateMachine = new EnemyStateMachine();
	this->graphics = Graphics::GetInstance();

}

Enemy::Enemy()
{
	
}

Enemy::~Enemy()
{
	delete enemyStateMachine;
}

void Enemy::Initialize()
{
	graphics = Graphics::GetInstance();
	movementSpeed = 20.0f;

	health = 100.0f;

	damage = 22.0f;
	rotation = { 0,0,0 }; 
	
	radius = 1.0f;

	isAlive = false;
}

void Enemy::Release()
{
	enemyStateMachine->Release();
}

void Enemy::Update(double deltaTime)
{
	enemyStateMachine->Update(deltaTime);
	renderInfo = { position, rotation };
}

float Enemy::GetHealth()
{
	return this->health;
}

void Enemy::SetHealth(float health)
{
	this->health = health;
}

void Enemy::Render()
{
	renderInfo = { position, rotation };
	graphics->QueueRender(&renderInfo);
}

void Enemy::Respawn(XMFLOAT3 spawn)
{
	this->position = spawn;
	this->isAlive  = true;
}

XMFLOAT3 Enemy::GetPosition() 
{ 
	return this->position; 
}

float Enemy::GetRadius() 
{
	return this->radius; 
}

void Enemy::AIPattern(Player * player, double deltaTime)
{
	XMFLOAT3 playerPos = player->GetPosition();
	Vec3 vect;

	vect.x = playerPos.x - position.x;
	vect.z = playerPos.z - position.z;
	vect.Normalize();
	this->position.x +=  vect.x *(float)deltaTime * movementSpeed;
	this->position.z +=  vect.z *(float)deltaTime * movementSpeed;
}