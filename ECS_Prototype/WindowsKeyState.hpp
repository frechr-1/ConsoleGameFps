#pragma once
#include "Windows.h"
#include <string>
#include "IKeyState.hpp"
#include <unordered_map>

class WindowsKeyState : public IKeyState {
private:
    HWND hConsoleWindow;
    HWND hHiddenWindow;
    std::unordered_map<wchar_t, bool> * keyStates;
    static WindowsKeyState * instance;
    static WNDPROC g_originalConsoleWndProc;
    static LRESULT CALLBACK CustomConsoleWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    explicit WindowsKeyState(HWND consoleWindowHandle, std::wstring keys = L"wasd");
    HWND CreateHiddenWindow();
    ~WindowsKeyState();

public:
    static WindowsKeyState * getInstance(HWND consoleWindowHandle);
    bool isKeyPressed(wchar_t key) override;
};
