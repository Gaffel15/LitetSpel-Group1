#pragma once
#include "./GameState/GameState.h"
#include "./GameOverState/GameOverState.h"
#include "./MenuState/MenuState.h"
#include "WinState\WinState.h"
#include "../../../Enumerations/Enumerations.h"
#include "../AudioManager/AudioManager.h"
class MainStateMachine
{
public:
	MainStateMachine();
	virtual ~MainStateMachine();
	void Update(double deltaTime);
	void Render();
	void Initialize();
	float delay;
	float lastHighscore;
	void Release();
	
private:

	AudioManager* audioManager = nullptr;
	GameState* gameState;
	GameOverState* gameOverState;
	WinState* winState;
	MenuState* menuState;
	MainStates activeState;//enum
	EnemySpawn* enemySpawn;
	
	float timePassed;

};

