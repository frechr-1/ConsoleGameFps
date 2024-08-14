#pragma once
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include "Archetype.hpp"
#include "IArchetype.hpp"

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual const IArchetype*  getDependencies() const = 0; 
    virtual const bool isThreadSafe() const = 0;
    virtual void execute() = 0;
};
