////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);
	int GetMouseX();
	int GetMouseY();
	int GetMouseXDelta();
	int GetMouseYDelta();
	bool isLeftMouseDown();
	bool isRightMouseDown();

	bool IsEscapePressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsDPressed();
	bool IsRPressed();
	bool IsFPressed();
	bool IsZPressed();
	bool IsQPressed();
	bool IsEPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();
	bool IsSpacePressed();
	bool IsShiftPressed();

	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF3Toggled();
	bool IsF4Toggled();
	bool IsF5Toggled();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY, oldMouseX, oldMouseY;

	bool m_F1_released;
	bool m_F2_released;
	bool m_F3_released;
	bool m_F4_released;
	bool m_F5_released;
};

#endif