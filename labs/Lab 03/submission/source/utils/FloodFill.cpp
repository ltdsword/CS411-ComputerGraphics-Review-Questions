#include "../header/FillAlgo.h"
#include "../header/RGBColor.h"
#include <queue>
#include <utility>

// Iterative BFS flood fill — avoids stack overflow for large areas.
// Replaces every pixel of the interior color (sampled at the seed) with fillColor.
// boundaryColor is accepted for API uniformity with BoundaryFill/ScanLineFill.
void FloodFill(int x, int y, RGBColor fillColor, RGBColor boundaryColor)
{
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    RGBColor oldColor = GetPixel(x, y); // derive interior color from seed pixel
    if (IsSameColor(fillColor, oldColor)) return; // nothing to do

    std::queue<std::pair<int,int>> q;
    q.push({x, y});

    while (!q.empty())
    {
        auto [px, py] = q.front();
        q.pop();

        if (px < 0 || px >= w || py < 0 || py >= h) continue;

        RGBColor c = GetPixel(px, py);
        if (!IsSameColor(c, oldColor)) continue;

        PutPixel(px, py, fillColor);

        q.push({px - 1, py});
        q.push({px + 1, py});
        q.push({px, py - 1});
        q.push({px, py + 1});
    }

    glFlush(); // single flush after all pixels are written
}
