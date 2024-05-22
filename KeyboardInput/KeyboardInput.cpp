#pragma once
#include <windows.h>

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

struct NullComp {};
struct TranslateComponent : NullComp {
	float x;
	float y;
	float dir;
	float speed;
};


// Set up the console screen buffer
void setupConsoleBuffer(HANDLE & hConsole, COORD & bufferSize,
						CHAR_INFO *& screenBuffer, int width, int height)
{
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
										 CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	bufferSize = { (SHORT)width, (SHORT)height };
	screenBuffer = new CHAR_INFO[width * height];
	for ( int i = 0; i < width * height; ++i ) {
		screenBuffer[i].Char.UnicodeChar = L' ';
		screenBuffer[i].Attributes =
			FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
	}
}

// Render t  he buffer to the consoles
void renderConsoleBuffer(HANDLE hConsole, CHAR_INFO * screenBuffer,
						 COORD bufferSize, COORD bufferSizeZero)
{
	SMALL_RECT writeRegion = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
	WriteConsoleOutput(hConsole, screenBuffer, bufferSize, bufferSizeZero,
					   &writeRegion);
}

// Function to write a string to a console buffer at a specified location
void writeToBuffer(CHAR_INFO * screenBuffer, int width, int x, int y,
				   const std::wstring & text, WORD attributes)
{
	if ( y < 0 )
		return; // Invalid y coordinate, do nothing
	for ( size_t i = 0; i < text.length(); ++i ) {
		int idx = y * width + x + i;
		if ( x + i < 0 || x + i >= width )
			continue; // Ensure we stay within buffer bounds
		screenBuffer[idx].Char.UnicodeChar = text[i];
		screenBuffer[idx].Attributes = attributes;
	}
}

void clearArea(CHAR_INFO * screenBuffer, int width, int x, int y, int areaWidth,
			   int areaHeight, WORD backgroundAttributes)
{
	for ( int row = 0; row < areaHeight; ++row ) {
		wchar_t a = L'';
		for ( int col = 0; col < areaWidth; ++col ) {
			int idx = (y + row) * width + (x + col);
			screenBuffer[idx].Char.UnicodeChar = L' '; // Set character to space
			screenBuffer[idx].Attributes =
				backgroundAttributes; // Set background color
		}
	}
}

// Define a vector to store key states; using atomic for thread-safe
// read/write
std::vector<std::atomic<bool>> keyStates(256);

void keyInputHandler()
{
	while ( true ) {
		for ( int i = 0; i < 256; ++i ) {
			keyStates[i] =
				(GetAsyncKeyState(i) & 0x8001) !=
				0; // Check the most significant bit and the least significant bit
		}
		std::this_thread::sleep_for(
			std::chrono::milliseconds(10)); // Sleep to reduce CPU usage
		if ( keyStates[VK_ESCAPE] ) {
			break; // Exit loop if ESCAPE is pressed
		}
	}
}

// Function to update screen based on key press
void updateKeyPressDisplay(CHAR_INFO * screenBuffer,
						   const std::vector<std::atomic<bool>> & keyStates,
						   int keyCode, const std::wstring & message, int width,
						   int x, int y, WORD attributes)
{
	if ( keyStates[keyCode] ) {
		writeToBuffer(screenBuffer, width, x, y, message, attributes);
	} else {
		clearArea(screenBuffer, width, x, y, static_cast<int>(message.length()), 1,
				  attributes);
	}
}

int main()
{
	std::thread inputThread(keyInputHandler); // Start the input handling thread

	const int width = 120;
	const int height = 30;
	HANDLE hConsole;
	COORD bufferSize;
	CHAR_INFO * screenBuffer;

	setupConsoleBuffer(hConsole, bufferSize, screenBuffer, width, height);
	COORD bufferSizeZero = { 0, 0 };

	// Main application logic here
	std::wstring aPress = L"A is pressed.";
	std::wstring dPress = L"D is pressed.";
	std::wstring wPress = L"W is pressed.";
	std::wstring sPress = L"S is pressed.";

	while ( !keyStates[VK_ESCAPE] ) { // Keep running until ESCAPE is pressed
		updateKeyPressDisplay(screenBuffer, keyStates, 'A', L"A is pressed.", width,
							  0, 10,
							  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		updateKeyPressDisplay(screenBuffer, keyStates, 'D', L"D is pressed.", width,
							  0, 15,
							  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		updateKeyPressDisplay(screenBuffer, keyStates, 'W', L"W is pressed.", width,
							  0, 20,
							  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		updateKeyPressDisplay(screenBuffer, keyStates, 'S', L"S is pressed.", width,
							  0, 25,
							  FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

		// Render the buffer to the console
		renderConsoleBuffer(hConsole, screenBuffer, bufferSize, bufferSizeZero);
	}

	inputThread.join(); // Wait for the input thread to finish
	std::cout << "Exiting program..." << std::endl;
	delete[] screenBuffer;
	CloseHandle(hConsole);
	return 0;
}