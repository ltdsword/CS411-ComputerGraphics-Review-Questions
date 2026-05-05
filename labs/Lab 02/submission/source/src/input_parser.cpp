#include "class/input_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

InputParser::InputParser() {}

std::vector<InputParser::ParsedObject> InputParser::parseFile(const std::string& filename) {
    std::vector<ParsedObject> objects;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open input file: " << filename << "\n";
        return objects;
    }
    
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        ParsedObject obj;
        
        // Read type
        if (!(iss >> obj.type)) {
            std::cerr << "Warning: Invalid type at line " << lineNumber << "\n";
            continue;
        }
        
        // Read parameters
        int param;
        while (iss >> param) {
            obj.params.push_back(param);
        }
        
        // Validate and add
        if (validateObject(obj)) {
            objects.push_back(obj);
        } else {
            std::cerr << "Warning: Invalid object at line " << lineNumber 
                     << " (type " << obj.type << ")\n";
        }
    }
    
    file.close();
    std::cout << "Parsed " << objects.size() << " objects from " << filename << "\n";
    
    return objects;
}

bool InputParser::validateObject(const ParsedObject& obj) {
    switch (obj.type) {
        case 0: // LineDDA
        case 1: // LineBresenham
            return obj.params.size() == 4; // X1 Y1 X2 Y2
        case 2: // CircleMidpoint
            return obj.params.size() == 3; // XT YT R
        case 3: // EllipseMidpoint
            return obj.params.size() == 4; // XT YT A B
        case 4: // ParabolaMidpoint
            return obj.params.size() == 3; // XT YT P
        case 5: // HyperbolaMidpoint
            return obj.params.size() == 4; // XT YT A B
        default:
            return false;
    }
}

std::string InputParser::ParsedObject::getDescription() const {
    std::string typeName;
    switch (type) {
        case 0: typeName = "Line DDA"; break;
        case 1: typeName = "Line Bresenham"; break;
        case 2: typeName = "Circle Midpoint"; break;
        case 3: typeName = "Ellipse Midpoint"; break;
        case 4: typeName = "Parabola Midpoint"; break;
        case 5: typeName = "Hyperbola Midpoint"; break;
        default: typeName = "Unknown"; break;
    }
    
    std::string desc = typeName + " (";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0) desc += " ";
        desc += std::to_string(params[i]);
    }
    desc += ")";
    
    return desc;
}
