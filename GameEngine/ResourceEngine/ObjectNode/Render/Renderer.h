#pragma once

#pragma region Includes
#include "../../../Source/LibIncluder.h"
#include "../../../Source/Constants.h"
#include "../../../Structs/RenderInfo.h"
#include "../../../Structs/DataTypes.h"
#include "../../ResourceManager/ResourceManager.h"
#include "..\Camera\Camera.h"
#include "../../../LightStructs.h"
#include "../../../Structs/ConstantBufferStruct.h"
#include "../../ResourceManager/LightManager/LightManager.h"
#pragma endregion

#define LIGHTBUFFER_AMOUNT 3
#define INSTANCED_BUFFER_AMOUNT 1


enum LIGHTBUFFERS {
	BUFFER_POINTLIGHTS,
	BUFFER_SPOTLIGHTS,
	BUFFER_DIRLIGHTS
};

enum INSTANCED_BUFFERS
{
	INSTANCED_WORLD

};

class Renderer
{

private:
	ID3D11DeviceContext* gDeviceContext						= nullptr;
	ID3D11Device * gDevice									= nullptr;
	ResourceManager* resourceManager						= nullptr;

	Camera* sceneCam										= nullptr;

	//Buffers
	ID3D11Buffer* worldBuffer								= nullptr;	   //world constBuffer
	ID3D11Buffer* cbufferPerFrame							= nullptr;	   
	ID3D11Buffer* sampleBoolsBuffer							= nullptr;	   //samplingState constBuffer (Controls if a mesh has normalmap,specmap, etc)
	
	ID3D11Buffer* instancedBuffers[INSTANCED_BUFFER_AMOUNT] = { nullptr };

	LightManager lightmanager;
	ID3D11Buffer* lightBuffers[LIGHTBUFFER_AMOUNT]			= { nullptr }; //Light constBuffers


	PointLightStruct* pointLightStruct						= nullptr;
	SpotLightStruct*  spotLightStruct						= nullptr;
	DirLightStruct*	  dirLightStruct						= nullptr;
	ID3D11ShaderResourceView* pointLightStructuredBuffer	= nullptr;
	ID3D11ShaderResourceView* spotLightStructuredBuffer		= nullptr;
	ID3D11ShaderResourceView* dirLightStructuredBuffer		= nullptr;

	XMFLOAT4 mouseWorldPos;

	int mNumPointLights = 0;
	int mNumSpotLights  = 0;
	int mNumDirLights	= 0;
public:
	Renderer();
	~Renderer();

	void Initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	void Release();
	
	void RenderBlurPass(ID3D11UnorderedAccessView* uav);
	void RenderFinalPass();
	void SetGbufferPass(bool x) { this->resourceManager->SetGbufferPass(x); };
	void SetShadowPass(bool x) { this->resourceManager->SetShadowPass(x);  };
	void Render(RenderInfoObject* object);
	void Render(RenderInfoUI* object);
	void Render(RenderInfoEnemy* object);
	void Render(RenderInfoChar* object);
	void Render(RenderInfoTrap* object);

	void RenderPlaceHolder(XMFLOAT3* position);
	void RenderPlaceHolder(XMFLOAT3* position, XMFLOAT3* rotation);
	void RenderPlaceHolderPlane();

	void RenderInstanced(RenderInfoEnemy* object, InstancedData* arrayData ,unsigned int amount);


	void SetMouseWorldPos(XMFLOAT4 position);

	void GetInverseViewMatrix(XMMATRIX &matrix);
	void GetInverseProjectionMatrix(XMMATRIX &matrix);

	
private:
	void Render(RenderInstructions* object);
	void RenderInstanced(RenderInstructions* object, ID3D11Buffer* instanceBuffer, unsigned int amount);
	
	void MapLightBufferStructures();
	void UpdateCbufferPerFrame();
	void UpdateLightBuffer();
	void UpdateWorldBuffer(WorldMatrix* worldStruct);
	void UpdateSampleBoolsBuffer(SampleBoolStruct* sampleStruct);
	bool CreateBuffers();

};
