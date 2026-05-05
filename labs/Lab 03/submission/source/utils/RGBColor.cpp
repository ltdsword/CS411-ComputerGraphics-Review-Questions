#include "../header/RGBColor.h"
#include <cstdlib>

// Window size — must match glutInitWindowSize in main.cpp
int w = 800, h = 600;

// Default fill color: Red; boundary color: White
RGBColor F_Color = {255, 0,   0  };
RGBColor B_Color = {255, 255, 255};

// ---- CPU-side software framebuffer -----------------------------------------
// glReadPixels returns stale background pixels on many Windows GPU drivers,
// even after glFinish(), making all pixel-readback approaches fail.
// Solution: maintain this CPU buffer independently of OpenGL readback.
//
//   SoftFillBackground() – initialise with the clear colour
//   SoftDrawLine()       – trace shape outlines via Bresenham
//   PutPixel()           – writes here AND to GL simultaneously
//   GetPixel()           – reads from here only; never calls glReadPixels
//
// Index mapping (model coordinates, y=0 at top):
//   idx = (h - y) * w * 3 + x * 3
// This matches the GL framebuffer layout (GL row 0 = physical bottom).
static unsigned char softbuf[600 * 800 * 3];

bool IsSameColor(RGBColor a, RGBColor b)
{
    const int tol = 5;
    return abs(a.r - b.r) < tol &&
           abs(a.g - b.g) < tol &&
           abs(a.b - b.b) < tol;
}

// Fill the entire software buffer with a solid colour (call after glClear).
void SoftFillBackground(RGBColor bg) {
    for (int i = 0; i < w * h * 3; i += 3) {
        softbuf[i]     = bg.r;
        softbuf[i + 1] = bg.g;
        softbuf[i + 2] = bg.b;
    }
}

// Bresenham line — draws from model (x0,y0) to (x1,y1) into softbuf.
// Called after GL draws the shape outline so the fill algorithm can
// detect the boundary pixels without any GPU readback.
void SoftDrawLine(int x0, int y0, int x1, int y1, RGBColor color) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    while (true) {
        if (x0 >= 0 && x0 < w && y0 > 0 && y0 < h) {
            int idx = (h - y0) * w * 3 + x0 * 3;
            softbuf[idx]     = color.r;
            softbuf[idx + 1] = color.g;
            softbuf[idx + 2] = color.b;
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

// Read a pixel from the CPU buffer (never touches the GPU).
RGBColor GetPixel(int x, int y) {
    if (x < 0 || x >= w || y <= 0 || y >= h) return {0, 0, 0};
    int idx = (h - y) * w * 3 + x * 3;
    return {softbuf[idx], softbuf[idx + 1], softbuf[idx + 2]};
}

// Write a pixel to the CPU buffer AND to GL.
// Updating softbuf here means the fill algorithm immediately sees the new
// colour on the very next GetPixel call — no GPU round-trip, no glFlush stall.
void PutPixel(int x, int y, RGBColor color) {
    if (x < 0 || x >= w || y <= 0 || y >= h) return;
    int idx = (h - y) * w * 3 + x * 3;
    softbuf[idx]     = color.r;
    softbuf[idx + 1] = color.g;
    softbuf[idx + 2] = color.b;
    glColor3ub(color.r, color.g, color.b);
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}