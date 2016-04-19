#pragma once
#include "../../Source/LibIncluder.h"
#include "../../Source/Constants.h"
#include "../../Enumerations/Enumerations.h"

class InputHandler
{
public:
	~InputHandler();
private:
	LPDIRECTINPUT8 input;
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;
	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;

	float mouseX, mouseY;
	float lastMouseX, lastMouseY;

private:
	InputHandler();
	void ProcessInput();
	bool ReadKeyboard();
	bool ReadMouse();

	//HWND hwndP;
public:
	bool Initialize(HWND* hwndP, HINSTANCE hInstance);
	void Release();

	bool IsKeyPressed(InputKeys* key);
	bool IsKeyHeld(InputKeys* key);
	DirectX::XMFLOAT2 GetMousePosition();
	bool isMouseClicked(InputKeys* mouseKey);

	static InputHandler* GetInstance();
	
};

