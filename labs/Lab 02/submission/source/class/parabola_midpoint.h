#pragma once

#include "object2d.h"

class ParabolaMidpoint : public Object2D {
private:
    int x0, y0;      // vertex coordinates
    int p;           // focal parameter (4p is the latus rectum)
    bool vertical;   // true for vertical axis, false for horizontal
    
    void plot2Symmetry(Renderer& renderer, int x, int y);

public:
    ParabolaMidpoint(int x0, int y0, int p, bool vertical = true);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
