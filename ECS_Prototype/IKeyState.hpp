#pragma once
#include <thread>
#include <atomic>
#include <unordered_map>
#include <functional>
#include "Windows.h"

class IKeyState {
public:
	virtual bool isKeyPressed(wchar_t) = 0;
};

class KeyState : public IKeyState {
private:
	std::unordered_map<wchar_t, std::atomic<bool>> keyStates;
	std::thread thread;
	std::atomic<bool> running{ true };
public:
	KeyState() {
		keyStates['D'] = false;
		keyStates['A'] = false;
		keyStates['W'] = false;
		keyStates['S'] = false;
		keyStates['Q'] = false;
		thread = std::thread(&KeyState::run, this);
	}

	~KeyState() {
		running = false;
		if(thread.joinable())
			thread.join();
	}

	void updateKeyState() {
		for (auto &key : keyStates) {
			key.second = (0x8000 & GetAsyncKeyState(key.first)) != 0;
		}
	}

	bool isKeyPressed(wchar_t key) override{
		return keyStates.find(key) != keyStates.end() && keyStates[key];
	}

	void run() {
		while (running) {
			while (!keyStates['Q']) {
				updateKeyState();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			running = false;
		}
	}
};