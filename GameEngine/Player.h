﻿#pragma once
#include "Character.h"
#include "LibIncluder.h"
#include "Input.h"
#include "ProjectileSystem.h"
#include "Enumerations.h"


class Player :
	public Character
{
public:
	Player();
	virtual ~Player();

	void Initialize();
	void Release();
	void Update(double deltaTime);
	void Render();
	void Move(MovementDirection dir, double deltaTime);

public:
	Input* userInput;


private:
	Meshes meshID;//enum
	ProjectileSystem* projectileSystem;

	//Collision();
};

