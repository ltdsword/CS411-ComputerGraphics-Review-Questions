#include "../header/helper.h"
#include <cstddef>

bool pointInPolygon(float px, float py, const std::vector<Point>& poly) {
    int n = (int)poly.size();
    if (n < 3) return false;
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        float xi = poly[i].x,  yi = poly[i].y;
        float xj = poly[j].x,  yj = poly[j].y;
        if (((yi > py) != (yj > py)) &&
            (px < (xj - xi) * (py - yi) / (yj - yi) + xi))
            inside = !inside;
    }
    return inside;
}

bool isPointInAnyPolygon(float x, float y) {
    for (size_t i = 0; i < polygons.size(); i++) {
        if (pointInPolygon(x, y, polygons[i].vertices)) {
            return true;
        }
    }
    if (isDrawing && pointInPolygon(x, y, currentPolygon.vertices)) {
        return true;
    }
    return false;
}