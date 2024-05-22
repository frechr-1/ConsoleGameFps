#pragma once
#include "SparseArray.hpp"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <optional>
#include "TranslateComponent.hpp"
#include "SymbolComponent.hpp" 

class ComponentManager {
protected:
	static std::unordered_map<std::type_index, void*> components;
public:
	ComponentManager(){
	}		
	~ComponentManager() {}

	template <typename C>
	C * getComponent(size_t entidx)
	{
		if(components.contains(typeid(C)) )
			return reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->get(entidx);
	}

	template <typename C>
	void addComponent(size_t entidx, C comp)
	{
		if ( components.contains(typeid(C))) {
			reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->add(entidx, comp);
		} else {
			components[typeid(C)] = reinterpret_cast<void*>(new SparseArray<C>());
			reinterpret_cast<SparseArray<C>*>(components[typeid(C)])->add(entidx, comp);
		}
	}

	template <typename C>
	void removeComponent(size_t entidx, C comp)
	{
		if (components.contains(typeid(C))) {
			auto comps = reinterpret_cast<SparseArray<C>*>(components[typeid(C)]);
			auto compToDelete = comps->get(entidx);
			if ( compToDelete != nullptr) {
				comps->remove(entidx);
			}
		}
	}

	static void cleanup()
	{
		for ( auto & pair : components ) {
			delete reinterpret_cast<SparseArray<void *> *>(pair.second); 
		}
		components.clear();
	}
};			
std::unordered_map<std::type_index, void *> ComponentManager::components;