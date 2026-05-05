#include "class/accuracy_calculator.h"
#include <cmath>
#include <algorithm>

// ============================================================================
// LineAccuracyCalculator
// ============================================================================

LineAccuracyCalculator::LineAccuracyCalculator(int x1, int y1, int x2, int y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {
    // Convert two-point form to general form: Ax + By + C = 0
    // From (y - y1)(x2 - x1) = (x - x1)(y2 - y1)
    // => (y2 - y1)x - (x2 - x1)y + (x2 - x1)y1 - (y2 - y1)x1 = 0
    
    A = static_cast<double>(y2 - y1);
    B = static_cast<double>(-(x2 - x1));
    C = static_cast<double>((x2 - x1) * y1 - (y2 - y1) * x1);
    
    normFactor = std::sqrt(A * A + B * B);
    if (normFactor == 0) normFactor = 1.0; // Prevent division by zero
}

double LineAccuracyCalculator::computeDistance(int x, int y) const {
    // Distance formula: d = |Ax + By + C| / sqrt(A^2 + B^2)
    return std::abs(A * x + B * y + C) / normFactor;
}

std::string LineAccuracyCalculator::getShapeName() const {
    return "Line";
}

// ============================================================================
// CircleAccuracyCalculator
// ============================================================================

CircleAccuracyCalculator::CircleAccuracyCalculator(int xc, int yc, int radius)
    : xc(xc), yc(yc), radius(radius) {}

double CircleAccuracyCalculator::computeDistance(int x, int y) const {
    // Distance error: e = |sqrt((x - xc)^2 + (y - yc)^2) - R|
    double dx = x - xc;
    double dy = y - yc;
    double distFromCenter = std::sqrt(dx * dx + dy * dy);
    return std::abs(distFromCenter - radius);
}

std::string CircleAccuracyCalculator::getShapeName() const {
    return "Circle";
}

// ============================================================================
// EllipseAccuracyCalculator
// ============================================================================

EllipseAccuracyCalculator::EllipseAccuracyCalculator(int xc, int yc, int rx, int ry)
    : xc(xc), yc(yc), rx(rx), ry(ry) {}

double EllipseAccuracyCalculator::computeDistance(int x, int y) const {
    // Error metric: e = |(x - xc)^2/rx^2 + (y - yc)^2/ry^2 - 1|
    // This is not exact Euclidean distance but acceptable for raster graphics
    double dx = x - xc;
    double dy = y - yc;
    double term = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    return std::abs(term - 1.0);
}

std::string EllipseAccuracyCalculator::getShapeName() const {
    return "Ellipse";
}

// ============================================================================
// ParabolaAccuracyCalculator
// ============================================================================

ParabolaAccuracyCalculator::ParabolaAccuracyCalculator(int x0, int y0, int p, bool vertical)
    : x0(x0), y0(y0), p(p), vertical(vertical) {}

double ParabolaAccuracyCalculator::computeDistance(int x, int y) const {
    if (vertical) {
        // Vertical parabola: (x - x0)^2 = 4p(y - y0)
        // Error: |(x - x0)^2 - 4p(y - y0)|
        double dx = x - x0;
        double dy = y - y0;
        double error = std::abs(dx * dx - 4.0 * p * dy);
        // Normalize by dividing by 4|p| to get approximate distance in pixels
        return error / (4.0 * std::abs(p) + 1.0);
    } else {
        // Horizontal parabola: (y - y0)^2 = 4p(x - x0)
        double dx = x - x0;
        double dy = y - y0;
        double error = std::abs(dy * dy - 4.0 * p * dx);
        return error / (4.0 * std::abs(p) + 1.0);
    }
}

std::string ParabolaAccuracyCalculator::getShapeName() const {
    return "Parabola";
}

// ============================================================================
// HyperbolaAccuracyCalculator
// ============================================================================

HyperbolaAccuracyCalculator::HyperbolaAccuracyCalculator(int xc, int yc, int a, int b, bool vertical)
    : xc(xc), yc(yc), a(a), b(b), vertical(vertical) {}

double HyperbolaAccuracyCalculator::computeDistance(int x, int y) const {
    double dx = x - xc;
    double dy = y - yc;
    
    if (!vertical) {
        // Horizontal hyperbola: x^2/a^2 - y^2/b^2 = 1
        // Error: |x^2/a^2 - y^2/b^2 - 1|
        double term = (dx * dx) / (a * a) - (dy * dy) / (b * b);
        return std::abs(term - 1.0);
    } else {
        // Vertical hyperbola: y^2/a^2 - x^2/b^2 = 1
        double term = (dy * dy) / (a * a) - (dx * dx) / (b * b);
        return std::abs(term - 1.0);
    }
}

std::string HyperbolaAccuracyCalculator::getShapeName() const {
    return "Hyperbola";
}
