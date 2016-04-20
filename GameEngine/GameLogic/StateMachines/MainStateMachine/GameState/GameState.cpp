#include "GameState.h"



GameState::GameState()
{
	this->death = new MainDeathState();
	this->pause = new MainPausedState();
	this->player = new Player();
	this->input = Input::GetInstance();
	this->enemy = new Enemy();
	this->room1 = new Scene();

}


GameState::~GameState()
{
	delete this->death;
	delete this->pause;
	delete this->player;
	delete this->enemy;
	delete this->room1;
}

void GameState::Initialize()
{
	input->Initialize();
	player->Initialize();
	death->Initialize();
	pause->Initialize();
	death->isActive = false;
	pause->isActive = false;
	enemy->Initialize();
	room1->Initialize();
}

void GameState::Release()
{

	//RELEASA ALLT H�R!
}

void GameState::Update(double deltaTime)
{
	ProcessInput(&deltaTime);
	player->Update(deltaTime);
	enemy->Update(deltaTime);
	room1->Update(deltaTime);
}

void GameState::ProcessInput(double* deltaTime)
{

	if (death->isActive)
	{

	}
	else if (pause->isActive)
	{
		if (input->IsKeyPressed(KEY_ESC))
		{
			pause->isActive = false;
		}
	}
	else
	{
		if (input->IsKeyPressed(KEY_W))
		{
			player->Move(UP, deltaTime[0]);
		}
		else if (input->IsKeyPressed(KEY_S))
		{
			player->Move(DOWN, deltaTime[0]);
		}
		else if (input->IsKeyPressed(KEY_A))
		{
			player->Move(LEFT, deltaTime[0]);
		}
		else if (input->IsKeyPressed(KEY_D))
		{
			player->Move(RIGHT, deltaTime[0]);
		}
		else if (input->IsKeyPressed(KEY_ESC))
		{
			pause->isActive = true;
		}
	}
}

void GameState::Render()
{
	room1->Render();
	player->Render();
	gameUI->Render();
	enemy->Render();

	
}

void GameState::OnEnter()
{
}

void GameState::OnExit()
{
}
