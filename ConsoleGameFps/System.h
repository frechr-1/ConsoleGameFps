#pragma once
#include <vector>
#include <string>
#include <set>
#include <typeindex>
#include <memory> 
#include "Entity.h"

class System {
public: 
    // This function will be used to determine which components are required by the system
    virtual std::set<std::type_index> getRequiredComponents() const = 0;

    // Update function to be called every frame or system tick
    virtual void update() = 0;

    // Function to add an entity to the system
    void addEntity(std::shared_ptr<Entity> entity) {
        _entities.push_back(entity);
    }
    std::vector<std::shared_ptr<Entity>> _entities;  // Stores pointers to entities this system processes
};