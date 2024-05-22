#pragma once
#include <Windows.h>
#include <chrono>
#include <set>
#include <typeindex>
#include "Entity.h" 
#include "InputManager.h"

class Player : public Entity {
public:
	Player(FCoord position = FCoord(1.0, 1.0), wchar_t character = 'P') {
		addComponent(std::make_shared< PositionComponent>(position));
		addComponent(std::make_shared<ConsoleCharacterComponent>(character));
	}

	std::set<std::type_index> requiredSystems() const override {
		return std::set<std::type_index> { typeid(PhysicsSystem),typeid(ConsoleRenderer) };
	}
};
	//void render() override {
	//	_buffer[(int)round(pos._y) * (int)_screenWidth + (int)round(pos._x)] = _character;
	//}
	//void update() override {
	//	t2 = chrono::system_clock::now();
	//	chrono::duration<float> elapsedTime = t2 - t1;
	//	t1 = t2;
	//	float dt = elapsedTime.count();
	//	float pixelsPerSec = 100.0f * dt;
	//	// User input
	//	if (inputManager.IsKeyPressed('A')) {
	//		pos._x = max(pos._x - pixelsPerSec, 0.0f);
	//	}
	//	if (inputManager.IsKeyPressed('D')) {
	//		pos._x = min(pos._x + pixelsPerSec, (float)_screenWidth - 1.0f);
	//	}
	//	if (inputManager.IsKeyPressed('W')) {
	//		pos._y = max(pos._y - pixelsPerSec, 0.0f);
	//	}
	//	if (inputManager.IsKeyPressed('S')) {
	//		pos._y = min(pos._y + pixelsPerSec, (float)_screenHeight - 1.0f);
	//	}
	//	if (pos._x != lastPos._x || pos._y != lastPos._y) {
	//		_buffer[(int)round(lastPos._y) * (int)_screenWidth + (int)round(lastPos._x)] = _bgChar;
	//		lastPos = pos;
	//	}
	//} 