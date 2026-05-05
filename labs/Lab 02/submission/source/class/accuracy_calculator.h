#pragma once

#include <set>
#include <utility>
#include <cmath>
#include <string>

// Base class for accuracy calculators
class AccuracyCalculator {
public:
    virtual ~AccuracyCalculator() = default;
    
    // Strategy B: Compute distance from a pixel to the ideal mathematical curve
    virtual double computeDistance(int x, int y) const = 0;
    
    // Get a descriptive name for this calculator
    virtual std::string getShapeName() const = 0;
};

// Line accuracy calculator
class LineAccuracyCalculator : public AccuracyCalculator {
private:
    int x1, y1, x2, y2;
    double A, B, C;  // Line equation: Ax + By + C = 0
    double normFactor; // sqrt(A^2 + B^2)
    
public:
    LineAccuracyCalculator(int x1, int y1, int x2, int y2);
    double computeDistance(int x, int y) const override;
    std::string getShapeName() const override;
};

// Circle accuracy calculator
class CircleAccuracyCalculator : public AccuracyCalculator {
private:
    int xc, yc, radius;
    
public:
    CircleAccuracyCalculator(int xc, int yc, int radius);
    double computeDistance(int x, int y) const override;
    std::string getShapeName() const override;
};

// Ellipse accuracy calculator
class EllipseAccuracyCalculator : public AccuracyCalculator {
private:
    int xc, yc, rx, ry;
    
public:
    EllipseAccuracyCalculator(int xc, int yc, int rx, int ry);
    double computeDistance(int x, int y) const override;
    std::string getShapeName() const override;
};

// Parabola accuracy calculator
class ParabolaAccuracyCalculator : public AccuracyCalculator {
private:
    int x0, y0, p;
    bool vertical;
    
public:
    ParabolaAccuracyCalculator(int x0, int y0, int p, bool vertical = true);
    double computeDistance(int x, int y) const override;
    std::string getShapeName() const override;
};

// Hyperbola accuracy calculator
class HyperbolaAccuracyCalculator : public AccuracyCalculator {
private:
    int xc, yc, a, b;
    bool vertical;
    
public:
    HyperbolaAccuracyCalculator(int xc, int yc, int a, int b, bool vertical = false);
    double computeDistance(int x, int y) const override;
    std::string getShapeName() const override;
};
