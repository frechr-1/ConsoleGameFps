#pragma once
#include "SparseArray.hpp"
#include "TranslateComponent.hpp"
#include "Debug.hpp"

class AIController {
private:
	AIController() {};
public:
	static inline void seekAndDestroy(MovementController * agentController, size_t * target, SparseArray<Translation> * translates)
	{
		// Check lateral difference, go up or down if target is above or below
		switch ( agentController->lateralComp(translates->get(*target)->y) ) {
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
		switch ( agentController->verticalComp(translates->get(*target)->x) ) {
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