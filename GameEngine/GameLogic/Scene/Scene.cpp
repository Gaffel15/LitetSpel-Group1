#include "Scene.h"
#include <time.h>

Scene::Scene()
{
	/*this->bearTraps = new std::vector<BearTrap>;
	this->fireTraps = new std::vector<FireTrap>;*/
	//	this->enemySpawn = new EnemySpawn;
	//this->trap = new Trap;
	collision = Collision::GetInstance();

}

Scene::~Scene()
{

	//delete this->enemySpawn;
	//	delete this->trap;


	for (size_t i = 0; i < enemySpawns.size(); i++)
	{
		delete this->enemySpawns.at(i);
	}

	/*delete this->sceneModels;
	delete this->sceneLights;
	delete this->enemySpawns;*/
	//delete this->bearTraps;
	//delete this->fireTraps;
	for (unsigned int i = 0; i < fireTraps.size(); i++)
	{
		if (fireTraps.at(i))
			delete fireTraps.at(i);
	}
	for (unsigned int i = 0; i < bearTraps.size(); i++)
	{
		if (bearTraps.at(i))
			delete bearTraps.at(i);
	}
	for (size_t i = 0; i < WeaponUpgrades.size(); i++)
	{
		if (WeaponUpgrades.at(i))
			delete WeaponUpgrades.at(i);
		
	}

}

void Scene::Initialize()
{
	trapAmount = 5;
	collision->ClearTraps();
	InitBearTrap();
	InitFireTrap();
	WeaponUpgrades.push_back(new Pickup(XMFLOAT3(10, 1, 0), PickupType::WEAPON));
	WeaponUpgrades.push_back(new Pickup(XMFLOAT3(-10, 1, 0), PickupType::HEAL));

	for (size_t i = 0; i < this->bearTraps.size(); i++)
	{
		collision->AddTrap(bearTraps.at(i));
	}

	RespawnTimer = 0;
}

void Scene::InitFireTrap()
{
	srand((unsigned int)time(0));


	for (int i = 0; i < trapAmount; i++)
	{
		XMFLOAT3 tmp; // randomizes the location of the beartrap
		tmp.x = rand() % 150 - 85.0f;
		tmp.y = 0;
		tmp.z = rand() % 150 - 65.0f;
		XMFLOAT3 pos = { tmp.x,tmp.y,tmp.z };
		fireTraps.push_back(new FireTrap(pos));
	}

}

void Scene::InitBearTrap()
{
	srand((unsigned int)time(0));


	for (int i = 0; i < trapAmount; i++)
	{
		XMFLOAT3 tmp; // randomizes the location of the beartrap
		tmp.x = rand() % 150 - 65.0f;
		tmp.y = 0;
		tmp.z = rand() % 150 - 85.0f;
		XMFLOAT3 pos = { tmp.x,tmp.y,tmp.z };
		BearTrap* temp = new BearTrap(pos);
		temp->Initialize(pos, temp->GetRotation());
		bearTraps.push_back(temp);
	}
}

void Scene::AddEnemySpawn(XMFLOAT3 spawnPosition)
{
	EnemySpawn* spawnPoint = new EnemySpawn();
	spawnPoint->Initialize(spawnPosition);
	enemySpawns.push_back(spawnPoint);
}

void Scene::Release()
{
	for (size_t i = 0; i < enemySpawns.size(); i++)
	{
		this->enemySpawns.at(i)->Release();
	}
}

void Scene::Update(double deltaTime)
{
	for (size_t i = 0; i < WeaponUpgrades.size(); i++)
	{
		WeaponUpgrades.at(i)->Update(deltaTime);
	}

	for (size_t i = 0; i < fireTraps.size(); i++)
	{
		fireTraps.at(i)->Update(deltaTime);

		if (fireTraps.at(i)->GetDot())
		{
			fireTraps.at(i)->GetDamage();
		}
	}

	for (size_t i = 0; i < bearTraps.size(); i++)
	{
		bearTraps.at(i)->Update(deltaTime);
		if (bearTraps.at(i)->GetSlow())
		{
			bearTraps.at(i)->GetDamage();
		}
	}

	for (size_t i = 0; i < enemySpawns.size(); i++)
	{
		enemySpawns.at(i)->Update(deltaTime);
	}

	for (size_t i = 0; i < bearTraps.size(); i++)
	{
		if (bearTraps.at(i)->GetState()->GetTrapState() != TrapState::TRAP_INACTIVE_STATE)
		{
			if (collision->BearTrapPlayerCollision(bearTraps.at(i)))
			{

				collision->PlayerProxyTrap(bearTraps.at(i));
				for (size_t j = 0; j < enemySpawns.size(); j++)
				{
					for (size_t k = 0; k < enemySpawns.at(j)->StandardAlive.size(); k++)
					{
						collision->EnemyProxTrap(bearTraps.at(i),
							enemySpawns.at(j)->StandardAlive.at(k))
							&& bearTraps.at(i)->isActive;
					}
				}
				bearTraps.at(i)->GetState()->SetTrapState(TrapState::TRAP_ACTIVE_STATE);

			}
			for (size_t j = 0; j < enemySpawns.size(); j++)
			{
				for (size_t k = 0; k < enemySpawns.at(j)->StandardAlive.size(); k++)
				{
					if (collision->BearTrapEnemyCollision(bearTraps.at(i),
						enemySpawns.at(j)->StandardAlive.at(k)))
					{
						collision->PlayerProxyTrap(bearTraps.at(i));
						for (size_t j2 = 0; j2 < enemySpawns.size(); j2++)
						{
							for (size_t k2 = 0; k2 < enemySpawns.at(j2)->StandardAlive.size(); k2++)
							{
								collision->EnemyProxTrap(bearTraps.at(i),
									enemySpawns.at(j2)->StandardAlive.at(k2))
									&& bearTraps.at(i)->isActive;
							}
						}
						bearTraps.at(i)->GetState()->SetTrapState(TrapState::TRAP_ACTIVE_STATE);
					}
				}
			}
		}
	}

	for (size_t i = 0; i < fireTraps.size(); i++)
	{
		if (fireTraps.at(i)->GetState()->GetTrapState() != TrapState::TRAP_INACTIVE_STATE)
		{
			if (collision->FireTrapPlayerCollision(fireTraps.at(i)) && fireTraps.at(i)->isActive)
			{
				fireTraps.at(i)->GetState()->SetTrapState(TrapState::TRAP_ACTIVE_STATE);
			}
			for (size_t j = 0; j < enemySpawns.size(); j++)
			{
				for (size_t k = 0; k < enemySpawns.at(j)->StandardAlive.size(); k++)
				{
					if (collision->FireTrapEnemyCollision(fireTraps.at(i),
						enemySpawns.at(j)->StandardAlive.at(k))
						&& fireTraps.at(i)->isActive)
					{
						fireTraps.at(i)->GetState()->SetTrapState(TrapState::TRAP_ACTIVE_STATE);
					}
				}
			}
		}
	}
}

void Scene::Render()
{
	for (size_t i = 0; i < fireTraps.size(); i++)
	{
		if (fireTraps.at(i)->isActive)
		{
			fireTraps.at(i)->Render();
		}

	}
	for (size_t i = 0; i < bearTraps.size(); i++)
	{
		if (bearTraps.at(i)->isActive)
		{
			bearTraps.at(i)->Render();
		}

	}

	for (size_t i = 0; i < WeaponUpgrades.size(); i++)
	{
		WeaponUpgrades.at(i)->Render();
	}

	//	enemySpawn->Render();


	for (size_t i = 0; i < enemySpawns.size(); i++)
	{

		enemySpawns.at(i)->Render();
	}

}

void Scene::load()
{

}

void Scene::EvadeTrap(EnemyBase* enemy, BearTrap* bear, double deltaTime)
{
	XMFLOAT3 enemyPos;
	XMFLOAT3 trapPos;
	float trapRadius;
	Vec3 dir;

	trapRadius = bear->GetRadius2();
	enemyPos = enemy->GetPosition();
	trapPos = bear->GetPosition();

	dir.x = trapPos.x - enemyPos.x;
	dir.z = trapPos.z - enemyPos.z;

	dir.Normalize();

	enemy->position.x -= dir.x * (float)deltaTime /* * enemy->movementSpeed*/;
	enemy->position.z -= dir.z * (float)deltaTime /** enemy->movementSpeed*/;

	/*enemy->position.x = dir.x += trapRadius;
	enemy->position.y = dir.y += trapRadius;*/
	/** (float)deltaTime * enemy->movementSpeed*/
	//enemy->position.z = trapRadius /** (float)deltaTime * enemy->movementSpeed*/;
}