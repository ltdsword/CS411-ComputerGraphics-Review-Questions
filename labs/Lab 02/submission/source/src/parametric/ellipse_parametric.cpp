#include "class/ellipse_parametric.h"
#include "class/renderer.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

EllipseParametric::EllipseParametric(int xc, int yc, int rx, int ry)
    : xc(xc), yc(yc), rx(rx), ry(ry) {}

void EllipseParametric::draw(Renderer& renderer) {
    if (rx <= 0 || ry <= 0) return;
    
    // Calculate number of points based on approximate perimeter
    // Ramanujan's approximation: π(3(a+b) - √((3a+b)(a+3b)))
    float h = std::pow((rx - ry), 2) / std::pow((rx + ry), 2);
    float perimeter = M_PI * (rx + ry) * (1 + 3 * h / (10 + std::sqrt(4 - 3 * h)));
    
    int numPoints = static_cast<int>(std::ceil(perimeter));
    if (numPoints < 16) numPoints = 16; // Minimum points
    
    float angleStep = 2 * M_PI / numPoints;
    
    // Draw ellipse using parametric equations
    // x = xc + rx*cos(θ)
    // y = yc + ry*sin(θ)
    for (int i = 0; i < numPoints; i++) {
        float theta = i * angleStep;
        float x = xc + rx * std::cos(theta);
        float y = yc + ry * std::sin(theta);
        
        renderer.drawPixel(static_cast<int>(std::round(x)), 
                          static_cast<int>(std::round(y)));
    }
}

void EllipseParametric::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string EllipseParametric::name() const {
    return "Ellipse Parametric";
}
