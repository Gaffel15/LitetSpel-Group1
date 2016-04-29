#include "InputHandler.h"



InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
	keyboard->Unacquire();
	mouse->Unacquire();
	input->Release();
}

void InputHandler::ProcessInput()
{
}

bool InputHandler::ReadKeyboard()
{
	return false;
}

bool InputHandler::ReadMouse()
{
	return false;
}

bool InputHandler::Initialize(HWND* hwndP ,HINSTANCE* hInstance)
{
	//keyboard->Acquire();
	this->hwndP = hwndP;

	HRESULT hr = DirectInput8Create(
		*hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&input,
		NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = input->CreateDevice(GUID_SysKeyboard, &this->keyboard, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	hr = input->CreateDevice(GUID_SysMouse,
		&mouse,
		NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	hr = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	hr = keyboard->SetCooperativeLevel(*hwndP, DISCL_BACKGROUND| DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	hr = mouse->SetCooperativeLevel(*hwndP, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (FAILED(hr))
	{
		return false;
	}


	hr = keyboard->Acquire();
	if (FAILED(hr))
	{
		hr = keyboard->Acquire();
		//Re-acquire helps
	}

	hr = mouse->Acquire();
	if (FAILED(hr))
	{
		hr = mouse->Acquire();
	}

	return true;
}

void InputHandler::Release()
{
}
	

bool InputHandler::IsKeyPressed(InputKeys* key)
{

	keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (*key == KEY_W && keyboardState[DIK_W])
	{
		return true;
	}
	else if (*key == KEY_A && keyboardState[DIK_A])
	{
		return true;
	}
	else if (*key == KEY_D && keyboardState[DIK_D])
	{
		return true;
	}
	else if (*key == KEY_S && keyboardState[DIK_S])
	{
		return true;
	}
	else if (*key == KEY_LEFT && keyboardState[DIK_LEFT])
	{
		return true;
	}
	else if (*key == KEY_SPACE && keyboardState[DIK_SPACE])
	{
		return true;
	}
	else if (*key == KEY_ENTER && keyboardState[DIK_RETURN])
	{
		return true;
	}
	else if (*key == KEY_P && keyboardState[DIK_P])
	{
		return true;
	}

	else if (*key == KEY_Z && keyboardState[DIK_Z])
	{
		return true;
	}
	else if (*key == KEY_X && keyboardState[DIK_X])
	{
		return true;
	}
	else if (*key == KEY_C && keyboardState[DIK_C])

	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputHandler::IsKeyHeld(InputKeys* key)
{
	return false;
}

XMFLOAT2 InputHandler::GetMousePosition()
{

	POINT point;
	ShowCursor(FALSE);

	GetCursorPos(&point);
	ScreenToClient(*this->hwndP, &point);
		
	mouseX = (float)point.x;
	mouseY = (float)point.y;

	if (GetCursorPos( &point))
	{

		ScreenToClient(*this->hwndP, &point);
		mouseX = (float)point.x;
		mouseY = (float)point.y;

		//if (ScreenToClient(*this->hwndP, &point))
		//{
		///*if (mouseX > WIN_WIDTH)
		//	{
		//		point.x = (LONG)WIN_WIDTH;
		//		ScreenToClient(*this->hwndP, &point);
		//		SetCursorPos(point.x, point.y);
		//	}
		//if (mouseY > WIN_HEIGHT)
		//	{
		//		point.y = (LONG)WIN_HEIGHT;
		//		ScreenToClient(*this->hwndP, &point);
		//		SetCursorPos(point.x, point.y);
		//	}

		//if (mouseX < 0)
		//{
		//	point.x = 0;
		//	ScreenToClient(*this->hwndP, &point);
		//	SetCursorPos(point.x, point.y);
		//}
		//if (mouseY < 0)
		//{
		//	point.y = 0;
		//	ScreenToClient(*this->hwndP, &point);
		//	SetCursorPos(point.x, point.y);
		//}*/
		//}
		
	}
	/*else

	*/

	return XMFLOAT2(mouseX, mouseY);
}

bool InputHandler::isMouseClicked(InputKeys* mouseKey)
{
	mouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&mouseState);

	if (*mouseKey == MOUSE_LEFT && mouseState.rgbButtons[0] & 0x80)
	{
	return true;
	}

	if (*mouseKey == MOUSE_RIGHT && mouseState.rgbButtons[1] & 0x80)
	{
		return true;
	}

	return false;
}

InputHandler * InputHandler::GetInstance()
{
	static InputHandler instance;
	return &instance;
}
