#pragma once

#include "object2d.h"

class CircleParametric : public Object2D {
private:
    int xc, yc, radius;

public:
    CircleParametric(int xc, int yc, int radius);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
