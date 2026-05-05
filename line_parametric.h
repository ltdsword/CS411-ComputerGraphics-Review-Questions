#pragma once

#include "object2d.h"

class LineParametric : public Object2D {
private:
    int x1, y1, x2, y2;

public:
    LineParametric(int x1, int y1, int x2, int y2);
    
    void draw(Renderer& renderer) override;
    void benchmark(Renderer& renderer, int iterations) override;
    std::string name() const override;
};
