#pragma once
#include "../../StateClass/StateClass.h"
class EnemyDeathState :
	public StateClass
{
private:
	void AIPattern();
	void OnEnter();
	void OnExit();

public:
	EnemyDeathState();
	~EnemyDeathState();
	void Initialize();
	void Release();
	void Update(double deltaTime);

};

