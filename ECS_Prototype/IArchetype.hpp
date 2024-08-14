#pragma once
#include <set>
#include <typeindex>

class IArchetype {
public:
    virtual ~IArchetype() = default;
    virtual std::set<std::type_index> componentIndicies() const = 0;
};
