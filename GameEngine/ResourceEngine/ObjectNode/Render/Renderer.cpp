#include "Renderer.h"



Renderer::Renderer()
{
	this->sceneCam			= new Camera();
	this->resourceManager	= new ResourceManager();
	this->sceneLightArray	= new PointLight(
		XMFLOAT4(0.0f, 30.0f, 0.0f, 1.0f), //Pos
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),	//Direction
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));	//Color
	sceneLightArray->SetMatrices(XM_PI*0.8f, 1.0f, 5.0f, 40.0f);
	sceneLightArray->intensity = 1.0f;
}


Renderer::~Renderer()
{
	delete sceneCam;
	delete resourceManager;
	delete sceneLightArray;
}

void Renderer::Initialize(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->gDeviceContext = gDeviceContext;
	this->gDevice = gDevice;
	this->CreateConstantBuffers();
	resourceManager->Initialize(gDevice, gDeviceContext);
	sceneCam->Initialize(gDevice, gDeviceContext);
	this->UpdateLightBuffer();
}
void Renderer::Release()
{
	resourceManager->Release();
	sceneCam->Release();

	SAFE_RELEASE(worldBuffer);
	SAFE_RELEASE(camBuffer);
	SAFE_RELEASE(lightBuffer);
	SAFE_RELEASE(sampleBoolsBuffer);

}

void Renderer::RenderFinalPass()
{

	RenderInstructions * objectInstruction;

	objectInstruction = this->resourceManager->GetFullScreenQuad();
	this->resourceManager->SetShader(Shaders::FINAL_SHADER);
	UINT32 vertexSize;

		vertexSize = sizeof(Vertex);

	UINT32 offset = 0;

	//an exception handling can be implemented here to handle if there is no buffer
	// to set. Then the handling can be to use a standard cube instead.

	this->gDeviceContext->IASetVertexBuffers(0, 1, &objectInstruction->vertexBuffer, &vertexSize, &offset);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetIndexBuffer(objectInstruction->indexBuffer, DXGI_FORMAT_R32_UINT, 0);



	this->gDeviceContext->DrawIndexed((UINT)*objectInstruction->indexCount, 0, 0);

}

#pragma region Overloaded Render functions

//Render scene objects, mostly static stuff
void Renderer::Render(RenderInfoObject * object)
{
	RenderInstructions* renderObject;

	//Send the info of the object into the resource manager
	//The resource manager gathers all the rendering info and sends back a renderInstruction
	renderObject = this->resourceManager->GetRenderInfo(object);

	//Render with the given render instruction

	this->Render(renderObject);

	//RenderPlaceHolder(&object->position,&object->rotation);

	//RenderPlaceHolder(&object->position);

}


//Render 2d textures for the ui
void Renderer::Render(RenderInfoUI * object)
{

}

//Render an enemy mesh
void Renderer::Render(RenderInfoEnemy * object)
{
	RenderInstructions * objectInstruction;

	objectInstruction = this->resourceManager->GetRenderInfo(object);


	Render(objectInstruction);
}


//Render the character, Update the camera to follow the position of the character
void Renderer::Render(RenderInfoChar * object)
{
	RenderInstructions * objectInstruction;
	
	objectInstruction = this->resourceManager->GetRenderInfo(object);

	//Update the camera view matrix!
	this->sceneCam->Updateview( object->position);
	this->UpdateCameraBuffer();

	Render(objectInstruction);

	

}


void Renderer::Render(RenderInfoTrap * object)
{
	RenderInstructions* renderObject;

	//Send the info of the object into the resource manager
	//The resource manager gathers all the rendering info and sends back a renderInstruction
	renderObject = this->resourceManager->GetRenderInfo(object);

	//Render with the given render instruction

	this->Render(renderObject);
}


//Render the placeholder meshes
void Renderer::RenderPlaceHolder(XMFLOAT3* position)
{
	RenderInstructions * object;
	object = this->resourceManager->GetPlaceHolderMesh( *position );
	
	Render(object);
}

void Renderer::RenderPlaceHolder(XMFLOAT3 * position, XMFLOAT3 * rotation)
{
	RenderInstructions * object;
	object = this->resourceManager->GetPlaceHolderMesh(*position,*rotation);



	Render(object);


}
void Renderer::RenderPlaceHolderPlane()
{
	RenderInstructions * objectPlane;
	objectPlane = this->resourceManager->GetPlaceHolderPlane();
	Render(objectPlane);

}
#pragma endregion

void Renderer::SetMouseWorldPos(XMFLOAT4 position)
{
	this->mouseWorldPos = position;

}

void Renderer::GetInverseViewMatrix(XMMATRIX & matrix)
{
	matrix = XMLoadFloat4x4(&this->sceneCam->GetCameraMatrices()->camView);
	matrix = XMMatrixTranspose(matrix);
	XMVECTOR det = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&det, matrix);
		
	
}

void Renderer::GetInverseProjectionMatrix(XMMATRIX & matrix)
{
	matrix = XMLoadFloat4x4(&this->sceneCam->GetCameraMatrices()->projection);
	matrix = XMMatrixTranspose(matrix); //Transpose to normal alignment.
	XMVECTOR det = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&det, matrix);

}

//Private rendering call
void Renderer::Render(RenderInstructions * object)
{

	UpdateLightBuffer();
	UpdateWorldBuffer(&object->worldBuffer);

#pragma region Check what vertex is to be used

	//We need to make sure that we use the right kind of vertex when rendering
	UINT32 vertexSize;
	
	if (*object->isAnimated		 == false)
		vertexSize = sizeof(Vertex);

	else if (*object->isAnimated == true)
		vertexSize = sizeof(AnimVert);

	else
		MessageBox(NULL, L"An object returned isAnimated as nullptr", L"Error in Renderer", MB_ICONERROR | MB_OK);
	
#pragma endregion


	UINT32 offset = 0;

	//an exception handling can be implemented here to handle if there is no buffer
	// to set. Then the handling can be to use a standard cube instead.

	this->gDeviceContext->IASetVertexBuffers(0, 1, &object->vertexBuffer, &vertexSize, &offset);
	
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetIndexBuffer(object->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	


#pragma region Set the objects texture maps to the shader

	SampleBoolStruct sampleBools;
	if (object->diffuseMap != nullptr)
	{
		this->gDeviceContext->PSSetShaderResources(0, 1, &object->diffuseMap);
		sampleBools.diffuseMap = TRUE;
	}

	if (object->normalMap != nullptr)
	{
		this->gDeviceContext->PSSetShaderResources(1, 1, &object->normalMap);
		sampleBools.normalMap = TRUE;
	}
	
	if (object->specularMap != nullptr)
	{
		this->gDeviceContext->PSSetShaderResources(2, 1, &object->specularMap);
		sampleBools.specularMap = TRUE;
	}

	if (object->glowMap != nullptr)
	{
		this->gDeviceContext->PSSetShaderResources(3, 1, &object->glowMap);
		sampleBools.glowMap = TRUE;
	}

	this->UpdateSampleBoolsBuffer(&sampleBools);
#pragma endregion
	
	
	this->gDeviceContext->DrawIndexed((UINT)*object->indexCount, 0, 0);


}

void Renderer::UpdateCameraBuffer()
{

	CamMatrices* tempCam				= this->sceneCam->GetCameraMatrices();
	tempCam->mousePos					= this->mouseWorldPos;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	gDeviceContext->Map(this->camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	CamMatrices* tempCamMatrices		= (CamMatrices*)mappedResource.pData;
	*tempCamMatrices					= *tempCam;
	
	

	gDeviceContext->Unmap(this->camBuffer, 0);
	
	gDeviceContext->GSSetConstantBuffers(CAMERABUFFER_INDEX, 1, &this->camBuffer);
	gDeviceContext->PSSetConstantBuffers(CAMERABUFFER_INDEX, 1, &this->camBuffer);



}

void Renderer::UpdateLightBuffer()
{

	PointLight* tempLight = this->sceneLightArray;
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	gDeviceContext->Map(this->lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	PointLight* tempLightData = (PointLight*)mappedResource.pData;
	*tempLightData = *tempLight;



	gDeviceContext->Unmap(this->lightBuffer, 0);

	gDeviceContext->GSSetConstantBuffers(LIGHTBUFFER_INDEX, 1, &this->lightBuffer);
	gDeviceContext->PSSetConstantBuffers(LIGHTBUFFER_INDEX, 1, &this->lightBuffer);

}

void Renderer::UpdateWorldBuffer(WorldMatrix* worldStruct)
{


	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	WorldMatrix* temporaryWorld = (WorldMatrix*)mappedResourceWorld.pData;

	*temporaryWorld = *worldStruct;

	

	this->gDeviceContext->Unmap(worldBuffer, 0);
	gDeviceContext->GSSetConstantBuffers(WORLDBUFFER_INDEX, 1, &this->worldBuffer);
	gDeviceContext->PSSetConstantBuffers(WORLDBUFFER_INDEX, 1, &this->worldBuffer);

}

void Renderer::UpdateSampleBoolsBuffer(SampleBoolStruct * sampleStruct)
{
	D3D11_MAPPED_SUBRESOURCE mappedResourceSampleBool;
	ZeroMemory(&mappedResourceSampleBool, sizeof(mappedResourceSampleBool));

	//mapping to the matrixbuffer
	this->gDeviceContext->Map(sampleBoolsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceSampleBool);

	SampleBoolStruct* temporaryStruct = (SampleBoolStruct*)mappedResourceSampleBool.pData;

	*temporaryStruct = *sampleStruct;



	this->gDeviceContext->Unmap(sampleBoolsBuffer, 0);
	gDeviceContext->PSSetConstantBuffers(SAMPLEBOOLSBUFFER_INDEX, 1, &this->sampleBoolsBuffer);

}

bool Renderer::CreateConstantBuffers()
{


	/* NOTE!!!
	
	The camera and world buffer are set to the geometry shader, the light buffer is set to the pixel shader
	*/
	HRESULT hr;

//-----------------------------------------------------------------------------------------------------------------------------------
		//CAMERA CONSTANT BUFFER
//-----------------------------------------------------------------------------------------------------------------------------------

	//Creating the Camera constant buffer
	CD3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth			 = sizeof(CamMatrices);
	bufferDesc.BindFlags			 = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage				 = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags		 = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			 = 0;
	bufferDesc.StructureByteStride	 = 0;

	hr = this->gDevice->CreateBuffer( &bufferDesc , nullptr , &camBuffer );
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create Camera buffer", L"Error", MB_ICONERROR | MB_OK);
	if (SUCCEEDED(hr))
	{

		this->gDeviceContext->GSSetConstantBuffers(CAMERABUFFER_INDEX, 1 , &this->camBuffer );
		this->gDeviceContext->PSSetConstantBuffers(CAMERABUFFER_INDEX, 1, &this->camBuffer);
	}

//-----------------------------------------------------------------------------------------------------------------------------------
		//WORLD CONSTANT BUFFER
//-----------------------------------------------------------------------------------------------------------------------------------

 //Creating world constant buffer																 
	CD3D11_BUFFER_DESC bufferDescWorld;
	ZeroMemory(&bufferDescWorld, sizeof(bufferDescWorld));

	bufferDescWorld.ByteWidth				 = sizeof(WorldMatrix);
	bufferDescWorld.BindFlags				 = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescWorld.Usage					 = D3D11_USAGE_DYNAMIC;
	bufferDescWorld.CPUAccessFlags			 = D3D11_CPU_ACCESS_WRITE;
	bufferDescWorld.MiscFlags				 = 0;
	bufferDescWorld.StructureByteStride		 = 0;

	hr = this->gDevice->CreateBuffer( &bufferDescWorld , nullptr , &worldBuffer );

	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create world buffer", L"Error", MB_ICONERROR | MB_OK);
	
	if (SUCCEEDED(hr))
		this->gDeviceContext->GSSetConstantBuffers(WORLDBUFFER_INDEX, 1, &worldBuffer); 

//-----------------------------------------------------------------------------------------------------------------------------------
		//LIGHT CONSTANT BUFFER
//-----------------------------------------------------------------------------------------------------------------------------------

	CD3D11_BUFFER_DESC bufferDescLight;
	ZeroMemory(&bufferDescLight, sizeof(bufferDescLight));

	bufferDescLight.ByteWidth				 = sizeof(PointLight);
	bufferDescLight.BindFlags				 = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescLight.Usage					 = D3D11_USAGE_DYNAMIC;
	bufferDescLight.CPUAccessFlags			 = D3D11_CPU_ACCESS_WRITE;
	bufferDescLight.MiscFlags				 = 0;
	bufferDescLight.StructureByteStride		 = 0;

	hr = this->gDevice->CreateBuffer(&bufferDescLight, nullptr, &lightBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
	if (SUCCEEDED(hr))
		this->gDeviceContext->PSSetConstantBuffers(	LIGHTBUFFER_INDEX, 1, &lightBuffer);


	//-----------------------------------------------------------------------------------------------------------------------------------
	// Sample Bool buffer (PS)CONSTANT BUFFER
	//-----------------------------------------------------------------------------------------------------------------------------------

	//This const buffer holds booleans, so that the gbuffer pass knows what textures the mesh has, so that it wont sample normal map if there is none.

	CD3D11_BUFFER_DESC bufferDescSample;
	ZeroMemory(&bufferDescSample, sizeof(bufferDescSample));

	bufferDescSample.ByteWidth				= sizeof(SampleBoolStruct);
	bufferDescSample.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	bufferDescSample.Usage					= D3D11_USAGE_DYNAMIC;
	bufferDescSample.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	bufferDescSample.MiscFlags				= 0;
	bufferDescSample.StructureByteStride	= 0;

	hr = this->gDevice->CreateBuffer(&bufferDescSample, nullptr, &sampleBoolsBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Failed to create sampleBools buffer", L"Error", MB_ICONERROR | MB_OK);
	if (SUCCEEDED(hr))
		this->gDeviceContext->PSSetConstantBuffers(SAMPLEBOOLSBUFFER_INDEX, 1, &sampleBoolsBuffer);




	return true;
}


