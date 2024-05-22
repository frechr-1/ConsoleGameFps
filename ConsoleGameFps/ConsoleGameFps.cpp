#pragma once
//#include <iostream>
//#include <vector>
//#include <Windows.h>
//#include <algorithm>
//#include "Entity.h"
#include "World.h" 
#include "Player.h"
//#include "System.h"
//#include "SystemManager.h"
//#include "PhysicsSystem.h"
//#include "ConsoleRenderer.h"

int main() {
	auto world = std::make_shared<World>();
	auto player = std::make_shared<Player>();
	world->add(player);
	while (true) {
		world->update();
	}
	return 0;
}