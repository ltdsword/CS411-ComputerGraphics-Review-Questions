#include "class/ellipse_midpoint.h"
#include "class/renderer.h"

EllipseMidpoint::EllipseMidpoint(int xc, int yc, int rx, int ry)
    : xc(xc), yc(yc), rx(rx), ry(ry) {}

void EllipseMidpoint::plot4Symmetry(Renderer& renderer, int x, int y) {
    renderer.drawPixel(xc + x, yc + y);
    renderer.drawPixel(xc - x, yc + y);
    renderer.drawPixel(xc + x, yc - y);
    renderer.drawPixel(xc - x, yc - y);
}

void EllipseMidpoint::draw(Renderer& renderer) {
    int x = 0;
    int y = ry;
    
    // Region 1 decision parameter
    long long rx2 = static_cast<long long>(rx) * rx;
    long long ry2 = static_cast<long long>(ry) * ry;
    long long twoRx2 = 2 * rx2;
    long long twoRy2 = 2 * ry2;
    
    long long px = 0;
    long long py = twoRx2 * y;
    
    // Plot initial point
    plot4Symmetry(renderer, x, y);
    
    // Region 1: slope < -1
    long long p1 = ry2 - (rx2 * ry) + (rx2 / 4);
    
    while (px < py) {
        x++;
        px += twoRy2;
        
        if (p1 < 0) {
            // Select E pixel
            p1 += ry2 + px;
        } else {
            // Select SE pixel
            y--;
            py -= twoRx2;
            p1 += ry2 + px - py;
        }
        
        plot4Symmetry(renderer, x, y);
    }
    
    // Region 2: slope >= -1
    long long p2 = ry2 * (x * x + x) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    
    while (y > 0) {
        y--;
        py -= twoRx2;
        
        if (p2 > 0) {
            // Select S pixel
            p2 += rx2 - py;
        } else {
            // Select SE pixel
            x++;
            px += twoRy2;
            p2 += rx2 - py + px;
        }
        
        plot4Symmetry(renderer, x, y);
    }
}

void EllipseMidpoint::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string EllipseMidpoint::name() const {
    return "Ellipse Midpoint";
}
