#pragma once
#include "SparseArray.hpp"
#include <unordered_map>
#include <typeindex>
#include <memory>	  	    

class ComponentManager {
protected:
	static std::unordered_map<std::type_index, void*> components;
	static std::unordered_map<std::type_index, size_t> componentSizes;
public:
	ComponentManager(){
	}		
	~ComponentManager() {}

	template <typename C>
	static C * getComponent(size_t entidx)
	{
		if(components.contains(typeid(C)) )
			return reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->get(entidx);
		else {
			return nullptr;
		}
	}

	template <typename C>
	static void addComponent(size_t entidx, C comp)
	{
		if ( components.contains(typeid(C))) {
			reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->add(entidx, comp);
			incrementComponentCount<C>();
		} else {
			components[typeid(C)] = reinterpret_cast<void*>(new SparseArray<C>());
			reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->add(entidx, comp);
			componentSizes[std::type_index(typeid(C))] = 1;
		}
	}

	template <typename C>
	static void removeComponent(size_t entidx)
	{
		if (components.contains(typeid(C))) {
			auto comps = reinterpret_cast<SparseArray<C>*>(components[typeid(C)]);
			auto compToDelete = comps->get(entidx);
			if ( compToDelete != nullptr) {
				comps->remove(entidx);
				decrementComponentCount<C>();
			}
		}
	}

	static size_t getSizeByTypeIndex(const std::type_index & idx)
	{
		auto it = componentSizes.find(idx);
		if ( it != componentSizes.end() ) {
			return it->second;
		}
		return 0;
	}
	
	template<typename C>
	static SparseArray<C> * getSparseArray()
	{
		if ( components.contains(typeid(C)) )
			return reinterpret_cast<SparseArray<C>*>(components[typeid(C)]);
		else {
			return nullptr;
		}
	}

	template <typename C>
	static void incrementComponentCount()
	{
		componentSizes[std::type_index(typeid(C))] += 1;
	}

	template <typename C>
	static void decrementComponentCount()
	{
		componentSizes[std::type_index(typeid(C))] -= 1;
	}

	template<typename C>
	static size_t getSize()
	{
		return reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->getLength();
	}

	template <typename C>
	static void cleanup()
	{
		for ( auto & pair : components ) {
			delete reinterpret_cast<SparseArray<C> *>(pair.second); 
		}
		components.clear();
		componentSizes.clear();
	}
};			
std::unordered_map<std::type_index, void *> ComponentManager::components;