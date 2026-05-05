#pragma once
#include "RGBColor.h"

// Phase 5 — recursive 4-connected boundary fill (utils/BoundaryFill.cpp)
void BoundaryFill(int x, int y, RGBColor fillColor, RGBColor boundaryColor);

// Phase 6 — iterative BFS flood fill (utils/FloodFill.cpp)
// oldColor: the interior color at the seed point (sampled before calling)
void FloodFill(int x, int y, RGBColor fillColor, RGBColor oldColor);

// Phase 7 — scan-line polygon fill (utils/ScanLine.cpp)
// Reads vertices from the global shapeVertices populated by the last Draw call.
void ScanLineFill(RGBColor fillColor);
