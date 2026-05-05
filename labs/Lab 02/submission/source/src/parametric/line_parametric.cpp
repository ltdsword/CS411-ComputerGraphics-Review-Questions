#include "class/line_parametric.h"
#include "class/renderer.h"
#include <cmath>

LineParametric::LineParametric(int x1, int y1, int x2, int y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

void LineParametric::draw(Renderer& renderer) {
    // Parametric form: P(t) = P1 + t(P2 - P1), where t ∈ [0, 1]
    float dx = static_cast<float>(x2 - x1);
    float dy = static_cast<float>(y2 - y1);
    
    // Calculate the length to determine number of steps
    float length = std::sqrt(dx * dx + dy * dy);
    int steps = static_cast<int>(std::ceil(length));
    
    if (steps == 0) {
        renderer.drawPixel(x1, y1);
        return;
    }
    
    // Draw points using parametric equation
    for (int i = 0; i <= steps; i++) {
        float t = static_cast<float>(i) / steps;
        float x = x1 + t * dx;
        float y = y1 + t * dy;
        
        renderer.drawPixel(static_cast<int>(std::round(x)), 
                          static_cast<int>(std::round(y)));
    }
}

void LineParametric::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string LineParametric::name() const {
    return "Line Parametric";
}
