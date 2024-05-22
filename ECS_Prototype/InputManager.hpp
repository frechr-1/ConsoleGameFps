#pragma once
#include <functional>
#include <unordered_map>
#include <tuple>
#include "windows.h"
#include "IKeyState.hpp"

class InputManager {
private:
    std::unordered_map<wchar_t, std::function<void()>> bindings;
    IKeyState & keyStates;

public:
    InputManager(IKeyState & ks) : keyStates(ks) {}

    void addBinding(wchar_t key, std::function<void()> func) {
        bindings.emplace(key, func);
    }

    void handleInput() {
        // Check the state of each key and call the bound function if the key is pressed
        for (auto& binding : bindings) {
            if (keyStates.isKeyPressed(binding.first)) {
                binding.second();
            }
        }
    }
}; 
