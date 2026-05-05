#pragma once

#include <string>
#include <vector>

class InputParser {
public:
    struct ParsedObject {
        int type;                    // 0-5: DDA, Bresenham, Circle, Ellipse, Parabola, Hyperbola
        std::vector<int> params;     // Parameters based on type
        
        std::string getDescription() const;
    };
    
    InputParser();
    
    // Parse input file and return all objects
    std::vector<ParsedObject> parseFile(const std::string& filename);
    
private:
    bool validateObject(const ParsedObject& obj);
};
