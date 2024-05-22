#pragma once

class SystemManager {
private:
	// A system is STATELESS
	// A system does not encapsulate another system.
	// Systems may operate on the same on the same entity, but then it should be though a thread safe interface...?
	// A system has a list of required components (size 0 or more)
	// Some entities might have shared systems... 
	// Maybe implement a mechanism for adding entities such that they give reference to each system, and also a priority? 
	//		RenderSystem=0,		
	//		PhysicsSystem=1, 
	//		MovementSystem/input=2, 
	//		Inventory=3
	//		AITargetting=4, etc..

public:
	SystemManager(){}

	template <typename System>
	void addSystem(System sys)
	{
		
	}
	void addEntity()
	{
		
	}
};