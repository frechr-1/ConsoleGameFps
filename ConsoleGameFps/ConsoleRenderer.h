#pragma once 
#include <Windows.h> 
#include "Entity.h" 
#include "System.h"


struct Settings {
	static const int SCREEN_WIDTH = 120;
	static const int SCREEN_HEIGHT = 30;
	static const char BG_CHAR = ' ';

};

class ConsoleRenderer : public System {
public:

	std::vector<wchar_t> screenBuffer;
	
	std::set<std::type_index> getRequiredComponents() const override {
		return {typeid(PositionComponent),typeid(ConsoleCharacterComponent)};
	}

	void update() override {
		for(auto ent : _entities) {
			int x = (int)round(ent->getComponent<PositionComponent>()->_position.x);
			int y = (int)round(ent->getComponent<PositionComponent>()->_position.y);
			wchar_t character = ent->getComponent<ConsoleCharacterComponent>()->_character;
			screenBuffer[y * _width + x] = character;
		}
		PresentBuffer();
	}

	void PresentBuffer() {
		if (!screenBuffer.empty() && hConsole != INVALID_HANDLE_VALUE) {
		WriteConsoleOutputCharacterW(hConsole, (LPCWSTR)screenBuffer.data(),
			_width * _height, { 0, 0 },
			&nBytesWritten);
		}
	}

	ConsoleRenderer(int width = Settings::SCREEN_WIDTH, int height = Settings::SCREEN_HEIGHT, char bgChar = Settings::BG_CHAR) :_width(width), _height(height), _bgChar(bgChar) {
		// Create screen buffer
		screenBuffer = std::vector<wchar_t>(height * width);
		hConsole = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hConsole);
		nBytesWritten = 0;
		std::fill(screenBuffer.begin(), screenBuffer.end(), bgChar);
		screenBuffer[width * height - 1] = L'\0';
	}

	~ConsoleRenderer() {
		if (hConsole != INVALID_HANDLE_VALUE) {
			CloseHandle(hConsole);
		}
	}
private:
	int _width;
	int _height;
	wchar_t _bgChar;
	DWORD nBytesWritten;
	HANDLE hConsole;
};