#pragma once
#include "WindowsKeyState.hpp"
#include "Debug.hpp"

WindowsKeyState* WindowsKeyState::instance = nullptr;
WNDPROC WindowsKeyState::g_originalConsoleWndProc = nullptr;

WindowsKeyState* WindowsKeyState::getInstance(HWND consoleWindowHandle) {
	if (WindowsKeyState::instance == nullptr) {
		WindowsKeyState::instance = new WindowsKeyState(consoleWindowHandle);
	}
	return WindowsKeyState::instance;
}

WindowsKeyState::WindowsKeyState(HWND consoleWindowHandle, std::wstring keys) : hConsoleWindow(consoleWindowHandle) {
	instance = this;
	keyStates = new std::unordered_map<wchar_t, bool>();
	for (auto& key : keys) {
		keyStates->emplace(key, false);
	}
	hHiddenWindow = CreateHiddenWindow();
	g_originalConsoleWndProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtrW(
			hConsoleWindow,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(CustomConsoleWndProc)
		)
		);

}

WindowsKeyState::~WindowsKeyState() {
	SetWindowLongPtr(hConsoleWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_originalConsoleWndProc));
}

LRESULT CALLBACK WindowsKeyState::CustomConsoleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	wchar_t key = static_cast<wchar_t>(wParam);
	switch (uMsg) {
	case WM_KEYDOWN:
		INFO("Keydown! " + uMsg);
		instance->keyStates->at(key) = true;
		break;
	case WM_KEYUP:
		instance->keyStates->at(key) = false;
		break;
	case WM_QUIT:
		exit(0);
		break;
	};

	return CallWindowProc(g_originalConsoleWndProc, hwnd, uMsg, wParam, lParam);
}
HWND WindowsKeyState::CreateHiddenWindow() {
	const char* CLASS_NAME = "HiddenWindowClass";
	WNDCLASS wc = {};
	wc.lpfnWndProc = CustomConsoleWndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = (LPCWSTR)CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,
		(LPCWSTR)CLASS_NAME,
		L"Hidden Control Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	return hwnd;
}

bool WindowsKeyState::isKeyPressed(wchar_t key) {
	if (keyStates->find(key) != keyStates->end())
		return keyStates->at(key);
	return false;
}
