#pragma once
#include "../Trap.h"
#include "../../../StateMachines/TrapStateMachine/TrapStateMachine.h"
class BearTrap :
	public Trap
{
public:
	BearTrap(XMFLOAT3 position);
	
	virtual ~BearTrap();
	void Initialize(XMFLOAT3 position, XMFLOAT3 rotation);
	void Release();
	void Update(double deltaTime);
	void Render();
	float GetSlow();
	float GetRadius2() { return this->radius2; }
	float GetRadius() { return this->radius; }
	XMFLOAT3 GetPosition() { return this->position; }
	void SetSlow(float slow);
	TrapStateMachine* GetState();



	//Collision();
private:
	RenderInfoTrap renderInfo;
	BearTrap();
	float slow;
	TrapStateMachine* trapState;
	float activeTimer;

	//RenderInfoTrap* renderinfo; 
	Graphics*  grapichs;

};

