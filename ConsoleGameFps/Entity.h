#pragma once
#include <vector>
#include <chrono>
#include <set>
#include <memory>
#include <typeindex> 
#include "Components.h" 

class Entity {
public:
    std::vector<IComponent> components;

    template<typename T>
    T getComponent() {
        for (auto comp : components) {
            T result =  T(comp);
            if (result)
            {
                return result;
            }
            else {
                
            }
        }
    }

    void addComponent( IComponent component) {
        components.push_back(component);
    }

    virtual std::set<std::type_index> requiredSystems() const = 0;
};