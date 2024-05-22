#pragma once
#include <iostream>
#include <queue>
#include "Windows.h"

class Logger {
private:
	HWND hMainConsole;
	HWND hConsole;
	HANDLE hPipeLogger;
	DWORD messageLength;
	DWORD bytesWritten;
	BOOL connected;
	void InitPipe() {
		hPipeLogger = CreateNamedPipe(
			L"\\\\.\\pipe\\Logger",
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE |
			PIPE_WAIT,
			1,
			4096,
			4096,
			0,
			NULL);

		if (hPipeLogger == INVALID_HANDLE_VALUE) {
			HWND hMainConsole = GetConsoleWindow();
			if (hMainConsole == INVALID_HANDLE_VALUE) {
				std::cerr << "Catastrophic failure, no console window found and logger pipe failed to initialize!" << std::endl;
			}
		}
		else {
			connected = ConnectNamedPipe(hPipeLogger, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		}
	}
public:
	Logger() {
		InitPipe();
		if (connected) {
			BOOL success = log("Logger initialized!\n");
			if (!success) std::cerr << "Logger is initialized and connected, but writing the message failed..." << std::endl;
		} else {
			std::cerr << "Couldn't connect to pipe." << std::endl;
		}
	}

	BOOL log(const char * msg) {
		if (!connected) {
			std::cerr << "Can't log without a pipe!" << std::endl; 
			return FALSE;
		}
		messageLength = strlen(msg);
		return WriteFile(
			hPipeLogger,
			msg,
			messageLength,
			&bytesWritten, 
			NULL);
	};
};