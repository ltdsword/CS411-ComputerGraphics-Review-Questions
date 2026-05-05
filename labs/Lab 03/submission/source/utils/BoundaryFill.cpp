#include "../header/FillAlgo.h"
#include "../header/RGBColor.h"
#include <stack>
#include <utility>

void BoundaryFill(int x, int y, RGBColor F_Color, RGBColor B_Color)
{
    if (x < 0 || x >= w || y < 0 || y >= h) return;

    std::stack<std::pair<int,int>> stk;
    stk.push({x, y});

    while (!stk.empty())
    {
        auto [px, py] = stk.top();
        stk.pop();

        if (px < 0 || px >= w || py < 0 || py >= h) continue;

        RGBColor cur = GetPixel(px, py);
        if (IsSameColor(cur, B_Color) || IsSameColor(cur, F_Color)) continue;

        PutPixel(px, py, F_Color);

        stk.push({px - 1, py});
        stk.push({px,     py + 1});
        stk.push({px + 1, py});
        stk.push({px,     py - 1});
    }

    glFlush();
}
