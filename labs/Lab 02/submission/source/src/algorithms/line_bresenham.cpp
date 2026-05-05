#include "class/line_bresenham.h"
#include "class/renderer.h"
#include <cmath>

LineBresenham::LineBresenham(int x1, int y1, int x2, int y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

void LineBresenham::draw(Renderer& renderer) {
    int x = x1, y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    
    // Determine direction of line
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    // Decision parameter
    int err = dx - dy;
    
    while (true) {
        renderer.drawPixel(x, y);
        
        // Check if we've reached the end point
        if (x == x2 && y == y2) break;
        
        // Calculate error for next pixel
        int e2 = 2 * err;
        
        // Step in x direction
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        
        // Step in y direction
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void LineBresenham::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string LineBresenham::name() const {
    return "Line Bresenham";
}
