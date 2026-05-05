#pragma once

#include "object2d.h"

class EllipseMidpoint : public Object2D {
private:
    int xc, yc, rx, ry;
    
    void plot4Symmetry(Renderer& renderer, int x, int y);

public:
    EllipseMidpoint(int xc, int yc, int rx, int ry);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
