#pragma once
#include <vector>

struct Point {
    float x, y;
};

struct PolygonShape {
    std::vector<Point> vertices;
    // Section 4 transformation states
    float tx = 0, ty = 0;
    float angle = 0;
    float scale = 1.0f;
    
    // Calculated center for affine transformations
    float cx = 0, cy = 0;

    void calculateCenter() {
        if (vertices.empty()) return;
        cx = 0; cy = 0;
        for (const auto& p : vertices) {
            cx += p.x;
            cy += p.y;
        }
        cx /= vertices.size();
        cy /= vertices.size();
    }
};

extern std::vector<PolygonShape> polygons;
extern PolygonShape currentPolygon;
extern bool isDrawing;
extern int hoveredPolygonIndex;
extern int selectedPolygonIndex;
extern float spawnX, spawnY;
extern int mainMenuID, selectMenuID;

// Point-in-polygon test
bool pointInPolygon(float px, float py, const std::vector<Point>& poly);
bool isPointInAnyPolygon(float x, float y);
