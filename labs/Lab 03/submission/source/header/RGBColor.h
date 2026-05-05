#pragma once
#include <GL/glut.h>

typedef struct _RGBColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGBColor;

// Window dimensions - defined in utils/RGBColor.cpp
extern int w, h;

// Global fill color and boundary color - defined in utils/RGBColor.cpp
extern RGBColor F_Color;
extern RGBColor B_Color;

bool     IsSameColor(RGBColor a, RGBColor b);
RGBColor GetPixel(int x, int y);
void     PutPixel(int x, int y, RGBColor color);

// Software-buffer helpers used by display() to set up the CPU framebuffer
// without relying on glReadPixels (broken on many Windows GPU drivers).
void SoftFillBackground(RGBColor bg);
void SoftDrawLine(int x0, int y0, int x1, int y1, RGBColor color);
