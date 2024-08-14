#pragma once
#include "SparseArray.hpp"
#include "TranslateComponent.hpp"
#include "Debug.hpp"
#include "SystemBase.hpp"	 
#include "Archetype.hpp"
#include "MovementController.hpp"

class AIController : public SystemBase<Translation>  {
private:
	size_t m_agent;
	size_t m_target;
public:	 

	AIController(size_t agent, size_t target = 0):m_agent(agent), m_target(target) {};
	const bool isThreadSafe() const override{ return false; }

	void execute() override
	{
		MovementController agentController(m_agent);
		// Check lateral difference, go up or down if target is above or below
		switch ( agentController->lateralComp(agentTranslates->get(*target)->y) ) {
			case 1:
				agentController->accelerateUp();
				break;
			case 0:
				// Target aligned laterally
				break;
			case -1:
				agentController->accelerateDown();
				break;
			default:
				ERROR("Laterally aligning agent failed.");
				break;
		}

		// Same as above but left and right
		switch ( agentController->verticalComp(agentTranslates->get(*target)->x) ) {
			case 1:
				agentController->accelerateRight();
				break;
			case 0:
				// Target alligned vertically
				break;
			case -1:
				agentController->accelerateLeft();
				break;
			default:
				ERROR("Vertically aligning agent failed.");
				break;
		}
	};
};