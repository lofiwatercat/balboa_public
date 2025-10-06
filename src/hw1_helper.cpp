#include "hw1_helper.h"

// Takes the canvas, center, radius, and color
void renderCircle(Image3 canvas, Vector2 center, Real radius, Vector3 color) {
    // Need to find the x and y inside the circle
    for (Real i = -center[0]/2; i < radius * 2; i++) {
        for (Real j = -center[1]/2; j < radius * 2; j++) {
            if (isInCircle(i, j, center, radius)) {
                canvas(i, j) = color;
            }
        }
    }
}

// Function to see if a pixel is within the circle
bool isInCircle(Real x, Real y, Vector2 center, Real radius) {
    // Distance from radius = sqrt(|x - center.x|^2 + |y - center.y|^2)
    Real distance = sqrt(pow(center[0] - x, 2) + pow(center[1] - y, 2));
    if (distance < radius) {
        return true;
    } else {
        return false;
    }
}
