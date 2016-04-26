#include "Collision.h"

Collision::Collision()
{

}

Collision* Collision::GetInstance()
{
	static Collision instance;
	return &instance;
}

void Collision::AddEnemy(Enemy* enemy)
{
	enemyBox.push_back(enemy);
}

void Collision::AddPlayer(Player* player)
{
	this->player = player;
}

void Collision::AddTrap(FireTrap * fTraps,BearTrap *bTrap)
{
	fireTrap.push_back(fTraps);
	bearTrap.push_back(bTrap);
}

bool Collision::BearTrapPlayerCollision(BearTrap * trap)
{

	XMFLOAT3 playPos = player->GetPosition();
	float playRad = player->GetRadius();
	trapPos = trap->GetPosition();
	trapRad = trap->GetRadius();

	if (pow(playPos.x - trapPos.x, 2)
		+ pow(playPos.z - trapPos.z, 2)
		< pow(playRad + trapRad, 2))
	{
		if (trap->isActive)
		{
			player->VelocityMax = 0.2f;
		}
		return true;
		
	}

	return false;
}

bool Collision::BearTrapEnemyCollision(BearTrap * trap, Enemy * enemy)
{
	trapPos = trap->GetPosition();
	trapRad = trap->GetRadius();

	enemyPos = enemy->GetPosition();
	enemyRad = enemy->GetRadius();

	if (pow(trapPos.x - enemyPos.x, 2)
		+ pow(trapPos.z - enemyPos.z, 2)
		< pow(trapRad + enemyRad, 2))
	{
		if (trap->isActive)
		{
			enemy->movementSpeed = 10.0f; // all hail the glorious satan
		}
		return true;
	}

	return false;
}

bool Collision::FireTrapPlayerCollision(FireTrap * trap)
{

	XMFLOAT3 playPos = player->GetPosition();
	float playRad = player->GetRadius();
	trapPos = trap->GetPosition();
	trapRad = trap->GetRadius();

	if (pow(playPos.x - trapPos.x, 2)
		+ pow(playPos.z - trapPos.z, 2)
		< pow(playRad + trapRad, 2))
	{
		if (trap->isActive)
		{
			player->DoT = trap->GetDot();
		}
 		return true;

	}
	return false;
}

bool Collision::FireTrapEnemyCollision(FireTrap * trap, Enemy * enemy)
{
	trapPos = trap->GetPosition();
	trapRad = trap->GetRadius();

	enemyPos = enemy->GetPosition();
	enemyRad = enemy->GetRadius();

	if (pow(trapPos.x - enemyPos.x, 2)
		+ pow(trapPos.z - enemyPos.z, 2)
		< pow(trapRad + enemyRad, 2))
	{
		if (trap->isActive)
		{
			enemy->DoT = trap->GetDot();
		}
		return true;
	}

	return false;
}

bool Collision::PlayerCollision(Enemy* enemy)
{
	XMFLOAT3 playPos = player->GetPosition();
	float playRad = player->GetRadius();
	
	enemyPos =  enemy->GetPosition();
	enemyRad = enemy->GetRadius();
	if (pow(playPos.x - enemyPos.x, 2) 
		+ pow(playPos.z - enemyPos.z, 2)
		< pow(playRad + enemyRad, 2)) 
	{
		if (enemy->isAlive)
		{
			player->SetHealth(player->GetHealth() - 10);
		}
		return true;
	}

	return false;
}

bool Collision::PlayerDistanceCollision(Enemy* enemy)
{
	XMFLOAT3 playPos = player->GetPosition();
	float playRad = player->GetRadius2();

	enemyPos = enemy->GetPosition();
	enemyRad = enemy->GetRadius2();

	if (pow(playPos.x - enemyPos.x, 2)
		+ pow(playPos.z - enemyPos.z, 2)
		< pow(playRad + enemyRad, 2))
	{
		return true;
	}
	return false;
}

bool Collision::ProjectileEnemyCollision(Projectile* projectile,Enemy* enemy)
{
	XMFLOAT3 proPos = projectile->GetPos();
	float proRad = projectile->GetRadius();

	enemyPos = enemy->GetPosition();
	enemyRad = enemy->GetRadius();
	if (pow(proPos.x - enemyPos.x, 2)
		+ pow(proPos.z - enemyPos.z, 2)
		< pow(proRad + enemyRad, 2))
	{
		return true;
	}

	return false;
}

bool Collision::EnemyCollision(Enemy* enemy, Enemy* enemys)
{
	enemyPos = enemy->GetPosition();
	enemyRad2 = enemy->GetRadius2();

	enemyPos2 = enemys->GetPosition();
	enemyRad3 = enemys->GetRadius2();

	if (pow(enemyPos2.x - enemyPos.x, 2)
		+ pow(enemyPos2.z - enemyPos.z, 2)
		< pow(enemyRad3 + enemyRad2, 2))
	{
		return true;
	}
	return false;
}

//
//bool Collision::TrapandEnemyLottery(BearTrap * trap, Enemy* enemys)
//{
//	trapPos = trap->GetPosition();
//	trapRad = trap->GetRadius2();
//
//	enemyPos = enemys->GetPosition();
//	enemyRad = enemys->GetRadius2();
//
//	if (pow(trapPos.x - enemyPos.x, 2)
//		+ pow(trapPos.z - enemyPos.z, 2)
//		< pow(trapRad + enemyRad, 2))
//	{
//		return true;
//	}
//	return false;
//}

Collision::~Collision()
{

}

Player* Collision::GetPlayer()
{
	return this->player;
}



