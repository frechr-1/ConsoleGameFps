#pragma once
#include "Windows.h"
#include <algorithm> 
#include <iostream> 
#include "TranslateComponent.hpp"
#include "SymbolComponent.hpp"
#include "Debug.hpp"
#include "fcntl.h"

template <size_t Width, size_t Height>
class ConsoleManager {
private:
public: 
	char bgChar_;
	HANDLE hConsole;
	HWND hwConsole;
	DWORD nBytesWritten;
	DWORD nAttributesWritten;
	CHAR_INFO consoleBuffer[Width*Height];
	SMALL_RECT psmallRect;

	ConsoleManager(wchar_t bgChar = L' ') :nBytesWritten(0), bgChar_(bgChar) {

	}

	~ConsoleManager() {
		CloseHandle(hConsole);
		FreeConsole();
	}

	HWND getWindowHandle() {
		if (!hwConsole || !IsWindow(hwConsole)) {
			ERROR("Invalid window handle!");
		} 
		return hwConsole;
	}

	void attachConsole()
	{
		// Allocate a console
		if ( !AllocConsole() ) {
			DWORD dwError = GetLastError();
			if ( dwError == ERROR_ACCESS_DENIED ) {
				WARNING("Console allocation failed: Access denied. A console already exists.");
			} else {
				ERROR("Console allocation failed with error code: " + dwError);
			}
		}

		// Redirect std::cout, std::cin, std::cerr, std::clog to the console
		FILE * fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
		std::cin.clear();
		std::cout.clear();
		std::cerr.clear();
		std::clog.clear();

		// Redirect C++ wide streams
		HANDLE hConIn = CreateFile(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hConOut = CreateFile(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SetStdHandle(STD_INPUT_HANDLE, hConIn);
		SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
		SetStdHandle(STD_ERROR_HANDLE, hConOut);

		// Make C++ streams point to console as well
		std::wcin.clear();
		std::wcout.clear();
		std::wcerr.clear();
		std::wclog.clear();

		// Sync with C I/O, required for scanf/printf redirection				   
		std::ios::sync_with_stdio();

		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		hwConsole = GetConsoleWindow();
		SetConsoleActiveScreenBuffer(hConsole);
		CHAR_INFO c ={ bgChar_, FOREGROUND_BLUE | FOREGROUND_GREEN };
		std::fill(consoleBuffer, consoleBuffer + Width * Height, c);
	}


	void Render() {
		/*WriteConsoleOutputCharacterW(hConsole, consoleBuffer,
			Width * Height, { 0, 0 },
			&nBytesWritten);*/
		//WORD attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
		//WriteConsoleOutputAttribute(hConsole, &attribute, (Width-1) * (Height -1), { 0,0 }, &nAttributesWritten);
		psmallRect.Bottom = Height;
		psmallRect.Left = 0;
		psmallRect.Right = Width;
		psmallRect.Top = 0;
		WriteConsoleOutputW(hConsole, consoleBuffer, { Width, Height }, { 0, 0 }, &psmallRect);
	}

	void UpdateBuffer(SparseArray<Translation>& translations, SparseArray<Symbol>& symbols) {
		for (int i = 0; i < translations.getLength(); i++) {
			Translation* t = translations.get(i);
			Symbol* s = symbols.get(i);
			// Delete previous symbol if position has changed.
			if (t->changed) {
				consoleBuffer[(static_cast<int>(round(t->yold)) * Width) + static_cast<int>(round(t->xold))].Char.UnicodeChar = ' ';
				consoleBuffer[(static_cast<int>(round(t->yold)) * Width) + static_cast<int>(round(t->xold))].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN;
				t->xold = t->x;
				t->yold = t->y;
				t->changed = false;
			} 
			consoleBuffer[(static_cast<int>(round(t->y)) * Width) + static_cast<int>(round(t->x)) ].Char.UnicodeChar = s->symbol;
			consoleBuffer[(static_cast<int>(round(t->y)) * Width) + static_cast<int>(round(t->x)) ].Attributes = FOREGROUND_GREEN;
		}
	}	  
};