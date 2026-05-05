#include "class/parabola_midpoint.h"
#include "class/renderer.h"

ParabolaMidpoint::ParabolaMidpoint(int x0, int y0, int p, bool vertical)
    : x0(x0), y0(y0), p(p), vertical(vertical) {}

void ParabolaMidpoint::plot2Symmetry(Renderer& renderer, int x, int y) {
    if (vertical) {
        renderer.drawPixel(x0 + x, y0 + y);
        renderer.drawPixel(x0 - x, y0 + y);
    } else {
        renderer.drawPixel(x0 + x, y0 + y);
        renderer.drawPixel(x0 + x, y0 - y);
    }
}

void ParabolaMidpoint::draw(Renderer& renderer) {
    if (p == 0) return;

    if (vertical) {
        // (x)^2 = 4p y
        int x = 0;
        int y = 0;

        // Decision parameter for region 1
        long long d = 1 - 2 * p;

        plot2Symmetry(renderer, x, y);

        // -------- Region 1: |x| < 2|p| --------
        while (abs(x) < 2 * abs(p)) {
            if (d < 0) {
                // Choose E
                d += 2 * x + 3;
            } else {
                // Choose NE
                y += (p > 0 ? 1 : -1);
                d += 2 * x + 3 - 4 * p;
            }
            x++;
            plot2Symmetry(renderer, x, y);
        }

        // -------- Region 2: |x| >= 2|p| --------
        d = (x + 1) * (x + 1) - 4 * p * (y + 1);

        while (abs(y) < abs(x) * 2) {
            if (d > 0) {
                // Choose N
                y += (p > 0 ? 1 : -1);
                d += -4 * p;
            } else {
                // Choose NE
                x++;
                y += (p > 0 ? 1 : -1);
                d += 2 * x + 2 - 4 * p;
            }
            plot2Symmetry(renderer, x, y);
        }

    } else {
        // (y)^2 = 4p x
        int x = 0;
        int y = 0;

        long long d = 1 - 2 * p;

        plot2Symmetry(renderer, x, y);

        // -------- Region 1 --------
        while (abs(y) < 2 * abs(p)) {
            if (d < 0) {
                d += 2 * y + 3;
            } else {
                x += (p > 0 ? 1 : -1);
                d += 2 * y + 3 - 4 * p;
            }
            y++;
            plot2Symmetry(renderer, x, y);
        }

        // -------- Region 2 --------
        d = (y + 1) * (y + 1) - 4 * p * (x + 1);

        while (abs(x) < abs(y) * 2) {
            if (d > 0) {
                x += (p > 0 ? 1 : -1);
                d += -4 * p;
            } else {
                y++;
                x += (p > 0 ? 1 : -1);
                d += 2 * y + 2 - 4 * p;
            }
            plot2Symmetry(renderer, x, y);
        }
    }
}

void ParabolaMidpoint::benchmark(Renderer& renderer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        draw(renderer);
    }
}

std::string ParabolaMidpoint::name() const {
    return "Parabola Midpoint";
}
