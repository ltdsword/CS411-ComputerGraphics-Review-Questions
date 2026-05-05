#include "../header/DrawShape.h"
#include "../header/RGBColor.h"
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <utility>

static const double PI = 3.14159265358979323846;

extern int w, h;

static inline float cx() { return w / 2.0f; }
static inline float cy() { return h / 2.0f; }

// ── vertex helpers ──────────────────────────────────────────────────────────
// addVertex records the point for ScanLineFill and passes it to OpenGL.
static void addVertex(float x, float y) {
    shapeVertices.push_back({x, y});
    glVertex2f(x, y);
}

// regularNgon clears the vertex list, then draws an n-sided polygon.
static void regularNgon(int n, float x0, float y0, float r, float a0 = 0.0f) {
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; i++) {
        double a = a0 + 2.0 * PI * i / n;
        addVertex(x0 + r * (float)cos(a),
                  y0 + r * (float)sin(a));
    }
    glEnd();
}

// ── 1. Duong thang ─────────────────────────────────────────────────────────
void DrawLine() {
    shapeVertices.clear(); // a line has no polygon interior
    glBegin(GL_LINES);
        glVertex2f(cx() - 60, cy() - 60);
        glVertex2f(cx() + 60, cy() + 60);
    glEnd();
}

// ── 2. Tam giac vuong can ───────────────────────────────────────────────────
void DrawTriangleRight() {
    float x = cx() - 60, y = cy() - 60, s = 120.0f;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(x,     y    );
        addVertex(x + s, y    );
        addVertex(x,     y + s);
    glEnd();
}

// ── 3. Tam giac deu ────────────────────────────────────────────────────────
void DrawTriangleEq() {
    regularNgon(3, cx(), cy(), 60.0f, -(float)PI / 2.0f);
}

// ── 4. Hinh chu nhat ───────────────────────────────────────────────────────
void DrawRectangle() {
    float hw = 60.0f, hh = 40.0f;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(cx() - hw, cy() - hh);
        addVertex(cx() + hw, cy() - hh);
        addVertex(cx() + hw, cy() + hh);
        addVertex(cx() - hw, cy() + hh);
    glEnd();
}

// ── 5. Hinh vuong ──────────────────────────────────────────────────────────
void DrawSquare() {
    float h2 = 55.0f;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(cx() - h2, cy() - h2);
        addVertex(cx() + h2, cy() - h2);
        addVertex(cx() + h2, cy() + h2);
        addVertex(cx() - h2, cy() + h2);
    glEnd();
}

// ── 6. Hinh tron ───────────────────────────────────────────────────────────
void DrawCircle() {
    regularNgon(360, cx(), cy(), 60.0f);
}

// ── 7. Elip ────────────────────────────────────────────────────────────────
void DrawEllipse() {
    float rx = 60.0f, ry = 40.0f;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        double a = 2.0 * PI * i / 360.0;
        addVertex(cx() + rx * (float)cos(a),
                  cy() + ry * (float)sin(a));
    }
    glEnd();
}

// ── 8. Ngu giac deu ────────────────────────────────────────────────────────
void DrawPentagon() {
    regularNgon(5, cx(), cy(), 60.0f, -(float)PI / 2.0f);
}

// ── 9. Luc giac deu ───────────────────────────────────────────────────────
void DrawHexagon() {
    regularNgon(6, cx(), cy(), 60.0f, 0.0f);
}

// ── 10. Mui ten ────────────────────────────────────────────────────────────
void DrawArrow() {
    float ox = cx() - 50, oy = cy();
    float sw = 80, sh = 20;
    float hw = 40, hh = 40;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(ox,           oy - sh);
        addVertex(ox + sw,      oy - sh);
        addVertex(ox + sw,      oy - hh);
        addVertex(ox + sw + hw, oy     );
        addVertex(ox + sw,      oy + hh);
        addVertex(ox + sw,      oy + sh);
        addVertex(ox,           oy + sh);
    glEnd();
}

// ── 11. Ngoi sao ───────────────────────────────────────────────────────────
void DrawStar() {
    float ro = 60.0f, ri = 24.0f;
    float start = -(float)PI / 2.0f;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 10; i++) {
        double a = start + PI * i / 5.0;
        float  r = (i % 2 == 0) ? ro : ri;
        addVertex(cx() + r * (float)cos(a),
                  cy() + r * (float)sin(a));
    }
    glEnd();
}

// ── 12. Dau cong (+) ──────────────────────────────────────────────────────
void DrawPlus() {
    float a = 45.0f, b = 20.0f;
    float x = cx(), y = cy();
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(x - b, y - a);
        addVertex(x + b, y - a);
        addVertex(x + b, y - b);
        addVertex(x + a, y - b);
        addVertex(x + a, y + b);
        addVertex(x + b, y + b);
        addVertex(x + b, y + a);
        addVertex(x - b, y + a);
        addVertex(x - b, y + b);
        addVertex(x - a, y + b);
        addVertex(x - a, y - b);
        addVertex(x - b, y - b);
    glEnd();
}

// ── 13. Dau tru (-) ───────────────────────────────────────────────────────
void DrawMinus() {
    float hw = 45.0f, hh = 10.0f;
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(cx() - hw, cy() - hh);
        addVertex(cx() + hw, cy() - hh);
        addVertex(cx() + hw, cy() + hh);
        addVertex(cx() - hw, cy() + hh);
    glEnd();
}

// ── 14. Dau nhan (×) ──────────────────────────────────────────────────────
// Thick × shape = Plus (+) polygon rotated 45 degrees.
// Each plus vertex (dx, dy) rotates to:
//   dx' = (dx - dy) / √2,  dy' = (dx + dy) / √2
// Pre-computed constants (a=45, b=20):
//   ao = (a+b)/√2 ≈ 45.96  (outer arm corner)
//   ai = (a-b)/√2 ≈ 17.68  (inner notch)
//   bm = 2b/√2   ≈ 28.28  (mid-arm waist)
void DrawMultiply() {
    const float a  = 45.0f, b = 20.0f;
    const float s  = 0.70711f; // 1/√2
    const float ao = (a + b) * s;
    const float ai = (a - b) * s;
    const float bm = 2.0f * b * s;
    float x = cx(), y = cy();
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(x + ai, y - ao);   //  1
        addVertex(x + ao, y - ai);   //  2
        addVertex(x + bm, y      );  //  3
        addVertex(x + ao, y + ai);   //  4
        addVertex(x + ai, y + ao);   //  5
        addVertex(x,      y + bm);   //  6
        addVertex(x - ai, y + ao);   //  7
        addVertex(x - ao, y + ai);   //  8
        addVertex(x - bm, y      );  //  9
        addVertex(x - ao, y - ai);   // 10
        addVertex(x - ai, y - ao);   // 11
        addVertex(x,      y - bm);   // 12
    glEnd();
}

// ── 15. Dau chia (/) ─────────────────────────────────────────────────────
// Thick forward-slash as a filled parallelogram:
// two parallel diagonal edges + flat top/bottom caps.
void DrawDivide() {
    float x = cx(), y = cy();
    float hw = 20.0f;   // half-width of the stroke (perpendicular thickness)
    float hl = 55.0f;   // half-length along each axis
    // Parallelogram vertices (clockwise from top-right):
    //   top-right, top-left, bottom-left, bottom-right
    shapeVertices.clear();
    glBegin(GL_LINE_LOOP);
        addVertex(x + hw + hl,  y - hl);   // top right
        addVertex(x - hw + hl,  y - hl);   // top left
        addVertex(x - hw - hl,  y + hl);   // bottom left
        addVertex(x + hw - hl,  y + hl);   // bottom right
    glEnd();
}


