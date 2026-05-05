#pragma once

#include <string>

class Renderer;

class Object2D {
public:
    virtual ~Object2D() = default;

    virtual void draw(Renderer& renderer) = 0;
    virtual void benchmark(Renderer& renderer, int iterations) = 0;
    virtual std::string name() const = 0;
};
