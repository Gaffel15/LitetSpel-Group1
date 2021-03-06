
#pragma once
#include "../Source/LibIncluder.h"

#include "../Enumerations/Enumerations.h"

#include "ConstantBufferStruct.h"


	using namespace DirectX;

struct RenderInstructions
{
	/*
	This struct only holds pointers to information
	When we want to render something. We send a set of instructions (see structs below) to the resource handler.
	Then the resource handler fills this struct with the information of the mesh that is to be rendered.
	Then this struct is returned to the Renderer that proceeds to update the buffers, resources and finally draws the geometry.
	*/
	unsigned int materialID;

	bool* isAnimated = nullptr; //this is important to make sure we use the right vert struct

	WorldMatrix worldBuffer;

	ID3D11Buffer* vertexBuffer = nullptr;
	UINT* vertexCount = 0;

	ID3D11Buffer* indexBuffer = nullptr;
	UINT* indexCount = 0;

	//UVoffset* uvIGuess = nullptr;

	ID3D11ShaderResourceView* diffuseMap = nullptr;
	ID3D11ShaderResourceView* normalMap = nullptr;
	ID3D11ShaderResourceView* specularMap = nullptr;
	ID3D11ShaderResourceView* glowMap = nullptr;
	bool glow = true;
};

struct RenderInfoObject {
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 direction;
	MeshEnum object;
	bool render = true;
	float radius;
};


struct RenderInfoUI {
	XMFLOAT3 size;
	XMFLOAT3 uv;
	UITextures UIobject;
	UiNumbers UInumber;
	//HUDEN Blyat;
	

	//HUDText texture;
	//Shaders shader;	//these are enum
	//float radius;

};

struct RenderInfoEnemy {
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	MeshEnum object;
	bool render				  = true;
	bool showHealthBar		  = false;
	bool isBeingHealed		  = false;
	float normalizedHealthVal = 1.0f; //this value is calculated  with currentHealth/maxHealth. This controls the size and color of the healthbar
	float radius;
	bool isOnfire = false;
	bool isSlowed = false;
	EnemyAnimations enemyAnim;	
	float animationTime; // 0.0 - 1.0
};

struct RenderInfoChar {
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 direction;
	float radius;
	float normalizedHealthVal = 1.0f;
	bool showHealthBar		  = false;
	bool isOnfire			  = false;
	bool isSlowed			  = false;
	//MeshEnum object;
	//Shaders shader;
	//PlayerAnimation playerAnim; //these are enum
};

struct RenderInfoTrap {
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	MeshEnum object;
	bool render = true;
	float radius;
	bool glow;
	float normalizedReloadVal = 1.0f; 
	//Shaders shader;
	//TrapAnimations trapAnim;*/ //these are enum
};
struct RenderInfoProjectile {
	XMFLOAT3 origin;
	XMFLOAT3 direction;
	float radius;

};
struct RenderInfoScene {
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	MeshEnum object;
	bool render = true;
	float radius;
};
