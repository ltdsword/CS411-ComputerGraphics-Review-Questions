#include "class/circle_parametric.h"
#include "class/renderer.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CircleParametric::CircleParametric(int xc, int yc, int radius)
    : xc(xc), yc(yc), radius(radius) {}

void CircleParametric::draw(Renderer& renderer) {
    if (radius <= 0) return;
    
    // Calculate number of points based on circumference
    // Use enough points to ensure good coverage
    int numPoints = static_cast<int>(std::ceil(2 * M_PI * radius));
    if (numPoints < 8) numPoints = 8; // Minimum points
    
    float angleStep = 2 * M_PI / numPoints;
    
    // Draw circle using parametric equations
    // x = xc + r*cos(θ)
    // y = yc + r*sin(θ)
    for (int i = 0; i < numPoints; i++) {
        float theta = i * angleStep;
        float x = xc + radius * std::cos(theta);
        float y = yc + radius * std::sin(theta);
        
        renderer.drawPixel(static_cast<int>(std::round(x)), 
                          static_cast<int>(std::round(y)));
    }
}

void CircleParametric::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string CircleParametric::name() const {
    return "Circle Parametric";
}
