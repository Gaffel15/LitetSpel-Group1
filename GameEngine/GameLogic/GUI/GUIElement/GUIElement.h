#pragma once
#include "../../../Source/LibIncluder.h"
#include "../../ResourceEngine/Graphics/Graphics.h"
#include "../../Structs/RenderInfo.h"
class GUIElement
{
public:
	GUIElement();
	virtual ~GUIElement();
	void Initialize();
	void Release();
	void Update(double deltaTime);
	void Render();
	RenderInfoUI renderInfo;
	Graphics* grapichs;

	void setUI(UITextures texture);
	void setUINR(UiNumbers numbers);
private:
	DirectX::XMFLOAT3 size;
	DirectX::XMFLOAT3 uv;

	UITextures texture;
	UiNumbers numbers;

	//DirectX::XMFLOAT3 position;
	//DirectX::XMFLOAT3 rotation;
};

