#pragma once
#include "RGBColor.h"

// Phase 5 — recursive 4-connected boundary fill (utils/BoundaryFill.cpp)
void BoundaryFill(int x, int y, RGBColor fillColor, RGBColor boundaryColor);

// Phase 6 — iterative BFS flood fill (utils/FloodFill.cpp)
// Derives the interior color from the seed pixel automatically.
void FloodFill(int x, int y, RGBColor fillColor, RGBColor boundaryColor);

// Phase 7 — scan-line polygon fill (utils/ScanLine.cpp)
// Reads vertices from the global shapeVertices populated by the last Draw call.
// x, y, boundaryColor are accepted for API uniformity but not used internally.
void ScanLineFill(int x, int y, RGBColor fillColor, RGBColor boundaryColor);
