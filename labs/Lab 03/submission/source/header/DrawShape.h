#pragma once
#include <vector>
#include <utility>

// Vertex list populated by every Draw call so ScanLineFill can use it.
extern std::vector<std::pair<float,float>> shapeVertices;

// Shape drawing functions — implemented in utils/DrawShape.cpp.
void DrawLine();
void DrawTriangleRight();
void DrawTriangleEq();
void DrawRectangle();
void DrawSquare();
void DrawCircle();
void DrawEllipse();
void DrawPentagon();
void DrawHexagon();
void DrawArrow();
void DrawStar();
void DrawPlus();
void DrawMinus();
void DrawMultiply();
void DrawDivide();
