#pragma once
#include <algorithm>
#include "TranslateComponent.hpp"
#include "SparseArray.hpp"

class MovementController {
private:
    Translation * translation; // Reference to the Translation component

public:
    MovementController(SparseArray<Translation> * translations, size_t entity) 
    {
        translation = translations->get(entity);
    }

    // Update function to handle movement
    void update() {
        // Update position based on velocity
        translation->x += translation->dx;
        translation->y += translation->dy;

        // Apply constraints
        translation->dx = std::clamp(translation->dx, -translation->dmax, translation->dmax);
        translation->dy = std::clamp(translation->dy, -translation->dmax, translation->dmax);

        // Apply drag
        translation->dx *= 0.75;
        translation->dy *= 0.75;

        if (translation->x != translation->xold || translation->y != translation->yold) {
            translation->changed = true;
        }
    }

    // Accelerate left
    void accelerateLeft() {
        translation->dx -= translation->inc;
    }

    // Accelerate right
    void accelerateRight() {
        translation->dx += translation->inc;
    }

    // Accelerate up
    void accelerateUp() {
        translation->dy -= translation->inc;
    }

    // Accelerate down
    void accelerateDown() {
        translation->dy += translation->inc;
    }

    // Check wether target is to the left (-1), to the right (1) or aligned (0)
    char verticalComp(float target_x)
    {
        if ( target_x < translation->x ) {
            return -1;
        } else if ( target_x == translation->x) {
            return 0;
        } else if ( target_x > translation->x ) {
            return 1;
        } else {
            ERROR("Something really hanky panky just went down!"); 
        }
    }

    // Check wether target is below (-1), above (1) or level(0)
    char lateralComp(float target_y)
    {
        if ( target_y < translation->y ) {
            return 1;
        } else if ( target_y == translation->y ) {
            return 0;
        } else if ( target_y > translation->y ) {
            return -1;
        } else {
            ERROR("Something really hanky panky just went down!"); 
        }
    }
};