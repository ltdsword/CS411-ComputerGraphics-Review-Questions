#include "class/parabola_parametric.h"
#include "class/renderer.h"
#include <cmath>

ParabolaParametric::ParabolaParametric(int x0, int y0, int p, bool vertical)
    : x0(x0), y0(y0), p(p), vertical(vertical) {}

void ParabolaParametric::draw(Renderer& renderer) {
    if (p == 0) return;
    
    // Draw parabola using parametric form
    // For vertical: x = x0 + t, y = y0 + t^2/(4p)
    // For horizontal: x = x0 + t^2/(4p), y = y0 + t
    
    int range = 100; // Drawing range
    float step = 1.0f;
    
    if (vertical) {
        // Vertical parabola: (x - x0)^2 = 4p(y - y0)
        for (float t = -range; t <= range; t += step) {
            float y = y0 + (t * t) / (4.0f * p);
            float x = x0 + t;
            
            if (std::abs(y - y0) <= range) {
                renderer.drawPixel(static_cast<int>(std::round(x)), 
                                  static_cast<int>(std::round(y)));
            }
        }
    } else {
        // Horizontal parabola: (y - y0)^2 = 4p(x - x0)
        for (float t = -range; t <= range; t += step) {
            float x = x0 + (t * t) / (4.0f * p);
            float y = y0 + t;
            
            if (std::abs(x - x0) <= range) {
                renderer.drawPixel(static_cast<int>(std::round(x)), 
                                  static_cast<int>(std::round(y)));
            }
        }
    }
}

void ParabolaParametric::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string ParabolaParametric::name() const {
    return "Parabola Parametric";
}
