#pragma once
#include "windows.h"
class InputManager {
public:
	InputManager() {};
	~InputManager() = default;
	bool IsKeyPressed(char key) { return GetAsyncKeyState((unsigned short)key) & 0x8000; };
};