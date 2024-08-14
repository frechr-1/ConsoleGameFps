#pragma once
#include "SparseArray.hpp"
#include "TranslateComponent.hpp"
#include "ISystem.hpp"

class PhysicsController : public ISystem {
private:
	PhysicsController() {};
	~PhysicsController() {};
public:
	PhysicsController(const PhysicsController &) = delete;
	PhysicsController & operator=(const PhysicsController &) = delete;
	
	static void updatePhysics(SparseArray<Translation> * tc)
	{
		auto i = 0, j = 0;
		auto last = tc->getLength();
		for ( ; i < last; i++ ) {
			auto t1 = tc->get(i);
			for ( j = i; j < last; j++ ) {
				auto t2 = tc->get(j);
				if ( round(t1->x) == round(t2->x) || round(t1->y) == round(t2->y)){
					// Rough check, now let's see which axis aligns the most and apply force by the largest dot product?
					/*t1->x = t1->x - t1->dx;
					t1->y = t1->y - t1->dy;


					t2->x = t2->x - t2->dx;
					t2->y = t2->y - t2->dy*/; 
				}
			}
		}
	}
};