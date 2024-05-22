#pragma once
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "TranslateComponent.hpp"
 
using Entity = size_t;
class Game {
protected:
	EntityManager em;
	ComponentManager cm;
	SystemManager sm;
public:
	Game() :em(), cm(), sm() {}
	~Game() {};			 
	
	void Init()
	{   
		Entity e = em.createEntity();
		cm.addComponent(e, Translation{});
		
	}
};