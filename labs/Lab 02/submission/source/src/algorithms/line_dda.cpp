#include "class/line_dda.h"
#include "class/renderer.h"
#include <cmath>

LineDDA::LineDDA(int x1, int y1, int x2, int y2) 
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

void LineDDA::draw(Renderer& renderer) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    // Determine the number of steps
    int steps = std::max(abs(dx), abs(dy));
    
    // Handle single point case
    if (steps == 0) {
        renderer.drawPixel(x1, y1);
        return;
    }
    
    // Calculate increments
    float xIncrement = static_cast<float>(dx) / steps;
    float yIncrement = static_cast<float>(dy) / steps;
    
    // Starting point
    float x = static_cast<float>(x1);
    float y = static_cast<float>(y1);
    
    // Draw all points
    for (int i = 0; i <= steps; i++) {
        renderer.drawPixel(static_cast<int>(round(x)), static_cast<int>(round(y)));
        x += xIncrement;
        y += yIncrement;
    }
}

void LineDDA::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string LineDDA::name() const {
    return "Line DDA";
}
