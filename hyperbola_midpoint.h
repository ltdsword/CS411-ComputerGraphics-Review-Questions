#pragma once

#include "object2d.h"

class HyperbolaMidpoint : public Object2D {
private:
    int xc, yc;      // center coordinates
    int a, b;        // semi-axes (a: transverse, b: conjugate)
    bool vertical;   // true for vertical transverse axis, false for horizontal
    
    void plot4Symmetry(Renderer& renderer, int x, int y);

public:
    HyperbolaMidpoint(int xc, int yc, int a, int b, bool vertical = false);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
