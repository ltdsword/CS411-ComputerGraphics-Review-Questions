#pragma once

#include "object2d.h"

class EllipseParametric : public Object2D {
private:
    int xc, yc, rx, ry;

public:
    EllipseParametric(int xc, int yc, int rx, int ry);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
