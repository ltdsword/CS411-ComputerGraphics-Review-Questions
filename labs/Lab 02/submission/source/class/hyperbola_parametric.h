#pragma once

#include "object2d.h"

class HyperbolaParametric : public Object2D {
private:
    int xc, yc;      // center coordinates
    int a, b;        // semi-axes
    bool vertical;   // true for vertical transverse axis, false for horizontal

public:
    HyperbolaParametric(int xc, int yc, int a, int b, bool vertical = false);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
