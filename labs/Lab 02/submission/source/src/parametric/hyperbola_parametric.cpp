#include "class/hyperbola_parametric.h"
#include "class/renderer.h"
#include <cmath>

HyperbolaParametric::HyperbolaParametric(int xc, int yc, int a, int b, bool vertical)
    : xc(xc), yc(yc), a(a), b(b), vertical(vertical) {}

void HyperbolaParametric::draw(Renderer& renderer) {
    if (a <= 0 || b <= 0) return;
    
    // Draw hyperbola using parametric form
    // Horizontal: x = xc ± a*cosh(t), y = yc + b*sinh(t)
    // Vertical: x = xc + b*sinh(t), y = yc ± a*cosh(t)
    
    float tMax = 3.5f; // Controls how far the branches extend
    float step = 0.05f;
    
    if (!vertical) {
        // Horizontal hyperbola: x^2/a^2 - y^2/b^2 = 1
        for (float t = 0; t <= tMax; t += step) {
            float coshT = std::cosh(t);
            float sinhT = std::sinh(t);
            
            // Right branch
            float x1 = xc + a * coshT;
            float y1 = yc + b * sinhT;
            float y2 = yc - b * sinhT;
            
            renderer.drawPixel(static_cast<int>(std::round(x1)), 
                              static_cast<int>(std::round(y1)));
            renderer.drawPixel(static_cast<int>(std::round(x1)), 
                              static_cast<int>(std::round(y2)));
            
            // Left branch
            float x2 = xc - a * coshT;
            renderer.drawPixel(static_cast<int>(std::round(x2)), 
                              static_cast<int>(std::round(y1)));
            renderer.drawPixel(static_cast<int>(std::round(x2)), 
                              static_cast<int>(std::round(y2)));
        }
    } else {
        // Vertical hyperbola: y^2/a^2 - x^2/b^2 = 1
        for (float t = 0; t <= tMax; t += step) {
            float coshT = std::cosh(t);
            float sinhT = std::sinh(t);
            
            // Top branch
            float y1 = yc + a * coshT;
            float x1 = xc + b * sinhT;
            float x2 = xc - b * sinhT;
            
            renderer.drawPixel(static_cast<int>(std::round(x1)), 
                              static_cast<int>(std::round(y1)));
            renderer.drawPixel(static_cast<int>(std::round(x2)), 
                              static_cast<int>(std::round(y1)));
            
            // Bottom branch
            float y2 = yc - a * coshT;
            renderer.drawPixel(static_cast<int>(std::round(x1)), 
                              static_cast<int>(std::round(y2)));
            renderer.drawPixel(static_cast<int>(std::round(x2)), 
                              static_cast<int>(std::round(y2)));
        }
    }
}

void HyperbolaParametric::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string HyperbolaParametric::name() const {
    return "Hyperbola Parametric";
}
