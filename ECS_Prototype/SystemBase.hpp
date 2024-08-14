#pragma once
#include "ISystem.hpp"
#include "Archetype.hpp"
#include "ComponentManager.hpp"

template<typename... Types>
class SystemBase :public ISystem
{
public:
	using Dependencies = Archetype<Types...>;
private:
	Dependencies dependencies;
	std::tuple<SparseArray<Types>*...> cachedArrays;
public:
	SystemBase()
	{
		initializeCaches();
	}
	const IArchetype * getDependencies() const override { return &dependencies; };
	const bool isThreadSafe() const override { return true; };
protected:
	template<typename C>
	C * getComponent(size_t entity)
	{
		return getComponentFromCache<C>(entity);
	}
private:
	void initializeCaches()
	{
		cachedArrays = std::make_tuple(ComponentManager::getSparseArray<Types>()...)
	}

	template<typename C>
	C * getComponentFromCache(size_t entity)
	{
		return std::get<SparseArray<C>*>(cachedArrays)->get(entity);
	}
};																  