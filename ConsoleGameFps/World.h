#pragma once
#include <vector>
#include <system.h>
#include <map>
#include <string>
#include <algorithm>
#include "entity.h"
#include "SystemManager.h"

class World {
public: 
	void add(std::shared_ptr<Entity> entity) {
		sm.registerEntity(entity);
	};

	void update() {
		sm.update();
	} 
	World(){
		sm = SystemManager();
	}; 
private:
	SystemManager sm;
};