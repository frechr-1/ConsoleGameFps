#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory> 
#include "Entity.h" 
#include "PhysicsSystem.h"
#include "ConsoleRenderer.h"
#include "System.h" 

class SystemManager {
private:
    std::unordered_map<std::type_index, System> systems;
public:
    void registerEntity(std::shared_ptr<Entity> entity) {
        std::set<std::type_index> required = entity->requiredSystems();
        for (auto sysType : required) {
            if (systems.find(sysType) == systems.end()) {
                // System does not exist, create and store it
                createSystem(sysType);
            }
            systems[sysType].addEntity(entity);
        }
    }

    // Update this whenever new systems are added to the architecture.
    void createSystem(std::type_index type) {
        // Factory pattern to create system based on type
        if (type == typeid(PhysicsSystem)) {
            systems[type] = PhysicsSystem();
        }
        else if (type == typeid(ConsoleRenderer)) {
            systems[type] =  ConsoleRenderer();
        }
    }

    void update() {
        for (auto pair : systems) {
            pair.second.update();
        }
    }
};
