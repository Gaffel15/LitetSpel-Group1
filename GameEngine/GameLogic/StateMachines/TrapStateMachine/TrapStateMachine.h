#pragma once
#include "./ActiveState/ActiveState.h"
#include "./IldeState/TrapIdleState.h"
#include "./InactiveState/InactiveState.h"
#include "../../../Enumerations/Enumerations.h"
//#include "../../Scene/Scene.h"
class TrapStateMachine
{
public:
	TrapStateMachine();
	virtual ~TrapStateMachine();

	void Initialize();
	void Release();
	void Update(double deltaTime);
	void Render();
	TrapState GetTrapState();
	void SetTrapState(TrapState state);
	
private:
	ActiveState* activeState;
	InactiveState* inactiveState;
	TrapIdleState* idleState;
	TrapState activeStateEnum;//enum
};

