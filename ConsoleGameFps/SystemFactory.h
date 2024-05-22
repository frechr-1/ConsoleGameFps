#pragma once
#include <functional>
#include "System.h"
#include "ConsoleRenderer.h"

class SystemFactory {
public:
    std::unique_ptr<System> createSystemForType(const std::type_index& type) {
        auto it = systemFactoryMap.find(type);
        if (it != systemFactoryMap.end()) {
            return it->second();
        }
        return nullptr;
    }

    void initializeSystemFactory() {
        systemFactoryMap[typeid(PhysicsComponent)] = []() -> std::unique_ptr<System> {
            return std::make_unique<PhysicsSystem>();
            };
        systemFactoryMap[typeid(ConsoleCharacterComponent)] = []() -> std::unique_ptr<System> {
            return std::make_unique<ConsoleRenderer>();
            };
    }
private:
    std::map<std::type_index, std::function<std::unique_ptr<System>()>> systemFactoryMap;
};
