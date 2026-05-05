#include "../header/FillAlgo.h"
#include "../header/DrawShape.h"
#include "../header/RGBColor.h"
#include <GL/glut.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

// Global vertex list — defined here, declared extern in header/DrawShape.h.
// Every DrawXxx() function clears and re-populates this before drawing.
std::vector<std::pair<float,float>> shapeVertices;

// Scan-line polygon fill.
// Works analytically from shapeVertices — does NOT rely on pixel boundary detection.
// x, y, boundaryColor are accepted for API uniformity with the other fill algorithms
// but are not used internally.
void ScanLineFill(int x, int y, RGBColor fillColor, RGBColor boundaryColor)
{
    int n = (int)shapeVertices.size();
    if (n < 3) return; // need at least a triangle

    // Compute vertex y range
    float yMinF = shapeVertices[0].second;
    float yMaxF = shapeVertices[0].second;
    for (auto& v : shapeVertices) {
        if (v.second < yMinF) yMinF = v.second;
        if (v.second > yMaxF) yMaxF = v.second;
    }
    int yMin = (int)std::ceil(yMinF);
    int yMax = (int)std::floor(yMaxF);

    for (int y = yMin; y <= yMax; y++)
    {
        std::vector<float> intersects;

        for (int i = 0; i < n; i++)
        {
            float x0 = shapeVertices[i].first,           y0 = shapeVertices[i].second;
            float x1 = shapeVertices[(i + 1) % n].first, y1 = shapeVertices[(i + 1) % n].second;

            if (y0 == y1) continue; // skip horizontal edges

            float yLo = std::min(y0, y1);
            float yHi = std::max(y0, y1);

            // Use half-open interval [yLo, yHi) to count each vertex once.
            if ((float)y < yLo || (float)y >= yHi) continue;

            float xIntersect = x0 + ((float)y - y0) * (x1 - x0) / (y1 - y0);
            intersects.push_back(xIntersect);
        }

        std::sort(intersects.begin(), intersects.end());

        // Fill between pairs of intersections
        for (int i = 0; i + 1 < (int)intersects.size(); i += 2)
        {
            int xStart = (int)std::ceil(intersects[i]);
            int xEnd   = (int)std::floor(intersects[i + 1]);
            for (int x = xStart; x <= xEnd; x++)
                PutPixel(x, y, fillColor);
        }
    }

    glFlush();
}
