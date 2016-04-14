#pragma once
#include "LibIncluder.h"
#include "Constants.h"

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
private:
	LPDIRECTINPUT8 input;
	LPDIRECTINPUTDEVICE8 keyboard;
	LPDIRECTINPUTDEVICE8 mouse;
	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;

	float mouseX, mouseY;
	float lastMouseX, lastMouseY;

private:
	void ProcessInput();
	bool ReadKeyboard();
	bool ReadMouse();

	HWND hwndP;
public:
	void Initialize();
	void Release();

	//bool IsKeyPressed(InputKeys* key);//need enums
	//bool IsKeyHeld(InputKeys* key);need enums
	DirectX::XMFLOAT2 GetMousePosition();
	//bool isMouseClicked(InputKeys* mouseKey;)need enums
	
};

