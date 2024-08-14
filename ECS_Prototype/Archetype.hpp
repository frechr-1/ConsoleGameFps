#pragma once
#include <set>
#include <typeindex>
#include <map>
#include "ComponentManager.hpp"
#include "IArchetype.hpp"

template<typename... Types>
class Archetype : public IArchetype {
private:
	std::set<std::type_index> archetype;
public:					   
	~Archetype() = default;

public:						   
	Archetype() 
	{
		(archetype.insert(std::type_index(typeid(Types))), ...);
	}

	std::set<std::type_index> componentIndicies() const override
	{
		return archetype;
	}

	auto begin() const
	{
		return archetype.begin();
	}

	auto end() const
	{
		return archetype.end();
	}

	bool operator ==(const Archetype & other) const
	{
		return archetype == other.archetype;
	}

	bool operator !=(const Archetype & other) const
	{
		return archetype != other.archetype;
	}
};