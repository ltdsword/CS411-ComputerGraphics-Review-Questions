#pragma once

#include "object2d.h"
#include <vector>

class ObjectFactory {
public:
    ObjectFactory();
    
    // Create custom algorithm object (DDA, Bresenham, Midpoint)
    Object2D* createCustomAlgorithm(int type, const std::vector<int>& params);
    
    // Create parametric equivalent for comparison (represents "OpenGL function")
    Object2D* createParametricAlgorithm(int type, const std::vector<int>& params);
    
    // Get algorithm name
    static std::string getAlgorithmName(int type);
};
