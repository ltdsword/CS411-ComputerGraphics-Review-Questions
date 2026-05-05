#include "class/object_factory.h"
#include "class/line_dda.h"
#include "class/line_bresenham.h"
#include "class/line_parametric.h"
#include "class/circle_midpoint.h"
#include "class/circle_parametric.h"
#include "class/ellipse_midpoint.h"
#include "class/ellipse_parametric.h"
#include "class/parabola_midpoint.h"
#include "class/parabola_parametric.h"
#include "class/hyperbola_midpoint.h"
#include "class/hyperbola_parametric.h"

ObjectFactory::ObjectFactory() {}

Object2D* ObjectFactory::createCustomAlgorithm(int type, const std::vector<int>& params) {
    switch (type) {
        case 0: // LineDDA
            if (params.size() == 4) {
                return new LineDDA(params[0], params[1], params[2], params[3]);
            }
            break;
        case 1: // LineBresenham
            if (params.size() == 4) {
                return new LineBresenham(params[0], params[1], params[2], params[3]);
            }
            break;
        case 2: // CircleMidpoint
            if (params.size() == 3) {
                return new CircleMidpoint(params[0], params[1], params[2]);
            }
            break;
        case 3: // EllipseMidpoint
            if (params.size() == 4) {
                return new EllipseMidpoint(params[0], params[1], params[2], params[3]);
            }
            break;
        case 4: // ParabolaMidpoint
            if (params.size() == 3) {
                return new ParabolaMidpoint(params[0], params[1], params[2], true);
            }
            break;
        case 5: // HyperbolaMidpoint
            if (params.size() == 4) {
                return new HyperbolaMidpoint(params[0], params[1], params[2], params[3], false);
            }
            break;
    }
    return nullptr;
}

Object2D* ObjectFactory::createParametricAlgorithm(int type, const std::vector<int>& params) {
    switch (type) {
        case 0: // LineDDA -> LineParametric
        case 1: // LineBresenham -> LineParametric
            if (params.size() == 4) {
                return new LineParametric(params[0], params[1], params[2], params[3]);
            }
            break;
        case 2: // CircleMidpoint -> CircleParametric
            if (params.size() == 3) {
                return new CircleParametric(params[0], params[1], params[2]);
            }
            break;
        case 3: // EllipseMidpoint -> EllipseParametric
            if (params.size() == 4) {
                return new EllipseParametric(params[0], params[1], params[2], params[3]);
            }
            break;
        case 4: // ParabolaMidpoint -> ParabolaParametric
            if (params.size() == 3) {
                return new ParabolaParametric(params[0], params[1], params[2], true);
            }
            break;
        case 5: // HyperbolaMidpoint -> HyperbolaParametric
            if (params.size() == 4) {
                return new HyperbolaParametric(params[0], params[1], params[2], params[3], false);
            }
            break;
    }
    return nullptr;
}

std::string ObjectFactory::getAlgorithmName(int type) {
    switch (type) {
        case 0: return "Line DDA";
        case 1: return "Line Bresenham";
        case 2: return "Circle Midpoint";
        case 3: return "Ellipse Midpoint";
        case 4: return "Parabola Midpoint";
        case 5: return "Hyperbola Midpoint";
        default: return "Unknown";
    }
}
