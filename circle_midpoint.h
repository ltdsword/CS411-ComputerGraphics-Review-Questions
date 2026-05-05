#pragma once

#include "object2d.h"

class CircleMidpoint : public Object2D {
private:
    int xc, yc, radius;
    
    void plot8Symmetry(Renderer& renderer, int x, int y);

public:
    CircleMidpoint(int xc, int yc, int radius);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
