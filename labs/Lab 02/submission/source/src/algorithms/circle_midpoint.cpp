#include "class/circle_midpoint.h"
#include "class/renderer.h"

CircleMidpoint::CircleMidpoint(int xc, int yc, int radius)
    : xc(xc), yc(yc), radius(radius) {}

void CircleMidpoint::plot8Symmetry(Renderer& renderer, int x, int y) {
    renderer.drawPixel(xc + x, yc + y);
    renderer.drawPixel(xc - x, yc + y);
    renderer.drawPixel(xc + x, yc - y);
    renderer.drawPixel(xc - x, yc - y);
    renderer.drawPixel(xc + y, yc + x);
    renderer.drawPixel(xc - y, yc + x);
    renderer.drawPixel(xc + y, yc - x);
    renderer.drawPixel(xc - y, yc - x);
}

void CircleMidpoint::draw(Renderer& renderer) {
    int x = 0;
    int y = radius;
    int p = 1 - radius;  // Initial decision parameter
    
    // Plot initial points using 8-way symmetry
    plot8Symmetry(renderer, x, y);
    
    while (x < y) {
        x++;
        
        if (p < 0) {
            // Midpoint is inside, select E pixel
            p += 2 * x + 1;
        } else {
            // Midpoint is outside or on circle, select SE pixel
            y--;
            p += 2 * (x - y) + 1;
        }
        
        plot8Symmetry(renderer, x, y);
    }
}

void CircleMidpoint::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string CircleMidpoint::name() const {
    return "Circle Midpoint";
}
