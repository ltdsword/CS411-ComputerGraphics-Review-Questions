#pragma once

#include "object2d.h"

class ParabolaParametric : public Object2D {
private:
    int x0, y0;      // vertex coordinates
    int p;           // focal parameter
    bool vertical;   // true for vertical axis, false for horizontal

public:
    ParabolaParametric(int x0, int y0, int p, bool vertical = true);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
