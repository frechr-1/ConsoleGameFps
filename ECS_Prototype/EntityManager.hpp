#pragma once
#include <array>
#include <stack>

// Wrapper for an array, but with extra logic regarding deletion and reuse of deleted entities.
class EntityManager {
using Entity = size_t;
private:
	const bool ACTIVE = true;
	const bool INACTIVE = false;  
	std::array<bool, 1000> entities;
	std::stack<Entity> availableIndices;
	size_t nextIndex = 0;

public:
	EntityManager() : entities{}, availableIndices{}, nextIndex(0), INACTIVE(false), ACTIVE(true)
	{
		entities.fill(INACTIVE);
	}

	Entity createEntity()
	{
		if ( availableIndices.empty() ) {
			entities.at(nextIndex) = ACTIVE;
			return nextIndex++;
		} else {
			size_t availableIdx = availableIndices.top();
			entities.at(availableIdx) = ACTIVE;
			availableIndices.pop();
			return availableIdx;
		}
	}

	void deleteEntity(Entity entity)
	{
		if ( entity >= nextIndex ) return;
		if ( entities.at(entity) == INACTIVE ) return;
		entities.at(entity) = INACTIVE;
		availableIndices.push(entity);
	}

	bool isActive(Entity entity) const
	{
		if ( entity < nextIndex ) {
			return entities[entity];
		}
		return false;
	}

	size_t capacity() const
	{
		return entities.max_size();
	}

	size_t size() const
	{
		return nextIndex - availableIndices.size();
	}
};