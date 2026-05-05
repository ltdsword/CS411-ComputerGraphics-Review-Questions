#include "class/hyperbola_midpoint.h"
#include "class/renderer.h"
#include <cmath>

HyperbolaMidpoint::HyperbolaMidpoint(int xc, int yc, int a, int b, bool vertical)
    : xc(xc), yc(yc), a(a), b(b), vertical(vertical) {}

void HyperbolaMidpoint::plot4Symmetry(Renderer& renderer, int x, int y) {
    if (vertical) {
        // Vertical: two branches (top and bottom)
        renderer.drawPixel(xc + x, yc + y);
        renderer.drawPixel(xc - x, yc + y);
        renderer.drawPixel(xc + x, yc - y);
        renderer.drawPixel(xc - x, yc - y);
    } else {
        // Horizontal: two branches (left and right)
        renderer.drawPixel(xc + x, yc + y);
        renderer.drawPixel(xc - x, yc + y);
        renderer.drawPixel(xc + x, yc - y);
        renderer.drawPixel(xc - x, yc - y);
    }
}

void HyperbolaMidpoint::draw(Renderer& renderer) {
    if (!vertical) {
        // Horizontal hyperbola: x^2/a^2 - y^2/b^2 = 1
        // Two branches: x >= a and x <= -a
        
        long long a2 = static_cast<long long>(a) * a;
        long long b2 = static_cast<long long>(b) * b;
        
        int x = a;
        int y = 0;
        
        plot4Symmetry(renderer, x, y);
        
        // Region 1: slope < 1
        long long d1 = b2 - a2 * b + a2 / 4;
        long long dx = 2 * b2 * x;
        long long dy = 2 * a2 * y;
        
        while (dx < dy + 2 * a2 * b) {
            if (d1 < 0) {
                // Select E pixel
                x++;
                dx += 2 * b2;
                d1 += dx + b2;
            } else {
                // Select NE pixel
                x++;
                y++;
                dx += 2 * b2;
                dy += 2 * a2;
                d1 += dx - dy + b2;
            }
            plot4Symmetry(renderer, x, y);
            
            if (x > a + 100) break; // Limit drawing
        }
        
        // Region 2: slope >= 1
        long long d2 = b2 * (x + 1) * (x + 1) - a2 * (y + 1) * (y + 1) - a2 * b2;
        
        while (y < 100) {
            if (d2 < 0) {
                // Select NE pixel
                x++;
                y++;
                dx += 2 * b2;
                dy += 2 * a2;
                d2 += dx - dy;
            } else {
                // Select N pixel
                y++;
                dy += 2 * a2;
                d2 += -dy;
            }
            plot4Symmetry(renderer, x, y);
            
            if (y > 100) break;
        }
        
    } else {
        // Vertical hyperbola: y^2/a^2 - x^2/b^2 = 1
        // Two branches: y >= a and y <= -a
        
        long long a2 = static_cast<long long>(a) * a;
        long long b2 = static_cast<long long>(b) * b;
        
        int x = 0;
        int y = a;
        
        plot4Symmetry(renderer, x, y);
        
        // Region 1
        long long d1 = b2 - a2 * b + a2 / 4;
        long long dx = 2 * b2 * x;
        long long dy = 2 * a2 * y;
        
        while (dy < dx + 2 * a2 * b) {
            if (d1 < 0) {
                // Select N pixel
                y++;
                dy += 2 * a2;
                d1 += dy + a2;
            } else {
                // Select NE pixel
                x++;
                y++;
                dx += 2 * b2;
                dy += 2 * a2;
                d1 += dy - dx + a2;
            }
            plot4Symmetry(renderer, x, y);
            
            if (y > a + 100) break;
        }
        
        // Region 2
        long long d2 = a2 * (y + 1) * (y + 1) - b2 * (x + 1) * (x + 1) - a2 * b2;
        
        while (x < 100) {
            if (d2 < 0) {
                // Select NE pixel
                x++;
                y++;
                dx += 2 * b2;
                dy += 2 * a2;
                d2 += dy - dx;
            } else {
                // Select E pixel
                x++;
                dx += 2 * b2;
                d2 += -dx;
            }
            plot4Symmetry(renderer, x, y);
            
            if (x > 100) break;
        }
    }
}

void HyperbolaMidpoint::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string HyperbolaMidpoint::name() const {
    return "Hyperbola Midpoint";
}
