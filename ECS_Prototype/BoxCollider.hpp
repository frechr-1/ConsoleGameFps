#pragma once
#include "SparseArray.hpp"
#include "TranslateComponent.hpp"
#include <cmath> 

struct Box {
	float x{ 0.0f }; float y{ 0.0f };
	float width{ 1.0f };
	float height{ 1.0f };
};

class BoxCollider {
private:
	BoxCollider() {}
	static inline bool isColliding(float ax, float  ay, float aw, float ah, float bx, float by, float bw, float bh)
	{
		// Check if box A and B are overlapping
		if ( ax < bx + bw && ax + aw > bx &&
			ay < by + bh && ay + ah > by ) {
			return true;
		}
		return false;
	}
	static inline void adjustVelocityOnCollision(Translation * a, Translation * b)
	{
		// Determine overlap
		float w = 5, h = 5;
		float overlapX = std::min(a->x + 1.0f, b->x + 1.0f) - std::max(a->x, b->x);
		float overlapY = std::min(a->y + 1.0f, b->y + 1.0f) - std::max(a->y, b->y);

		if ( overlapX > overlapY ) {
			// More significant overlap on Y axis, collision is horizontal
			float tempDy = a->dy;
			a->dy = b->dy;
			b->dy = tempDy;

			// Reflect the velocities
			a->dy = -a->dy;
			b->dy = -b->dy;

			a->y = a->yold;
			b->y = b->yold;
		} else {
			// More significant overlap on X axis, collision is vertical
			float tempDx = a->dx;
			a->dx = b->dx;
			b->dx = tempDx;

			// Reflect the velocities
			a->dx = -a->dx;
			b->dx = -b->dx;
			a->x = a->xold;
			b->x = b->xold;
		}
	}
public:
	static inline void checkCollisions(SparseArray<Translation> * translates)
	{
		auto i = 0, j = 0;
		auto last = translates->getLength();
		for ( ; i < last; i++ ) {
			for ( j = i; j < last; j++ ) {
				if ( BoxCollider::isColliding(translates->get(i)->x, translates->get(i)->y, 1.0f, 1.0f, translates->get(j)->x, translates->get(j)->y, 1.0f, 1.0f) ) {
					adjustVelocityOnCollision(translates->get(i), translates->get(j));
				}
			}
		}
	}
};