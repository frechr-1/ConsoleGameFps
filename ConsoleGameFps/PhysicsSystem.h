#pragma once 
#include <algorithm> 
#include "Entity.h"
#include "System.h"
#include "Components.h" 

class PhysicsSystem : public System {
public:
    std::set<std::type_index> getRequiredComponents() const override {
        return { typeid(PositionComponent), typeid(PhysicsComponent) };
    }
    void update() override {
         std::for_each(_entities.begin(), _entities.end(), [](auto e) {
            auto pos = e->getComponent<PositionComponent>();
            pos._position.y++;
            });  
    }
};