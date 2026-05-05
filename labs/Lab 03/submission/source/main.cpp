#include <GL/freeglut.h>
#include "header/RGBColor.h"
#include "header/DrawShape.h"
#include "header/FillAlgo.h"

// ─────────────────────────────────────────────────────────────
// Shape selector
// ─────────────────────────────────────────────────────────────
enum Shape {
    NONE,
    LINE,
    TRI_RIGHT, TRI_EQ,
    RECTANGLE, SQUARE,
    CIRCLE, ELLIPSE,
    PENTAGON, HEXAGON,
    ARROW, STAR,
    PLUS, MINUS, MULTIPLY, DIVIDE
};

int currentShape = NONE;

// Fill state — stored here so display() can re-run the fill on every redraw.
// FreeGLUT posts a redisplay when the right-click menu closes (the popup
// "dirtied" the window area), so display() is called after the user picks a
// colour and any fill drawn directly in XuLyMouse would be erased by glClear.
// Keeping the fill parameters here and re-running BoundaryFill inside
// display() ensures the filled scene survives all redraws.
static bool     fillApplied    = false;
static bool     colorChosen    = false;   // true only after user picks a colour
static int      fillStartX     = 0, fillStartY = 0;
static RGBColor savedFillColor = {255, 0, 0};

// ─────────────────────────────────────────────────────────────
// Display callback
// ─────────────────────────────────────────────────────────────
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw shape boundary
    glColor3ub(B_Color.r, B_Color.g, B_Color.b);

    switch (currentShape) {
        case LINE:      DrawLine();          break;
        case TRI_RIGHT: DrawTriangleRight(); break;
        case TRI_EQ:    DrawTriangleEq();    break;
        case RECTANGLE: DrawRectangle();     break;
        case SQUARE:    DrawSquare();        break;
        case CIRCLE:    DrawCircle();        break;
        case ELLIPSE:   DrawEllipse();       break;
        case PENTAGON:  DrawPentagon();      break;
        case HEXAGON:   DrawHexagon();       break;
        case ARROW:     DrawArrow();         break;
        case STAR:      DrawStar();          break;
        case PLUS:      DrawPlus();          break;
        case MINUS:     DrawMinus();         break;
        case MULTIPLY:  DrawMultiply();      break;
        case DIVIDE:    DrawDivide();        break;
        default: break;
    }

    // After GL draws the shape outline, build the CPU software buffer so
    // BoundaryFill can do pixel lookups without any GPU readback:
    //   1. Fill softbuf with the background colour.
    //   2. Trace the shape outline edges (held in shapeVertices) into softbuf
    //      using Bresenham lines, marking the boundary pixels.
    // GetPixel/PutPixel now operate purely on softbuf — no glReadPixels.
    SoftFillBackground({33, 33, 33});  // must match glClearColor
    {
        int n = (int)shapeVertices.size();
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            SoftDrawLine(
                (int)shapeVertices[i].first,  (int)shapeVertices[i].second,
                (int)shapeVertices[j].first,  (int)shapeVertices[j].second,
                B_Color);
        }
    }

    // Re-apply fill every time the scene is redrawn.  The fill state was saved
    // when the user left-clicked, so any redisplay triggered by FreeGLUT
    // (e.g. when the context menu closes) will correctly reproduce the fill.
    if (fillApplied) {
        BoundaryFill(fillStartX, fillStartY, savedFillColor, B_Color);
    }

    glFlush();
}

// ─────────────────────────────────────────────────────────────
// Point-in-polygon test (ray-casting) using shapeVertices.
// Returns false for degenerate shapes (< 3 vertices: Line, Minus, etc.)
// ─────────────────────────────────────────────────────────────
static bool pointInPolygon(float px, float py) {
    int n = (int)shapeVertices.size();
    if (n < 3) return false;
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        float xi = shapeVertices[i].first,  yi = shapeVertices[i].second;
        float xj = shapeVertices[j].first,  yj = shapeVertices[j].second;
        if (((yi > py) != (yj > py)) &&
            (px < (xj - xi) * (py - yi) / (yj - yi) + xi))
            inside = !inside;
    }
    return inside;
}

// ─────────────────────────────────────────────────────────────
// Mouse callback
// Left click → start BoundaryFill
// ─────────────────────────────────────────────────────────────
void XuLyMouse(int button, int state, int x, int y) {
    if (currentShape == NONE) return;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Require the user to choose a fill colour first.
        if (!colorChosen) return;
        // Ignore clicks outside the shape boundary — they would flood the
        // entire background since there is no enclosing boundary there.
        if (!pointInPolygon((float)x, (float)y)) return;

        // Save the fill point and colour, then ask GLUT to redraw.
        // display() will draw the shape outline first and then run
        // BoundaryFill — this way the fill survives any subsequent redraws
        // (e.g. the one FreeGLUT posts automatically when the menu closes).
        fillStartX     = x;
        fillStartY     = y;
        savedFillColor = F_Color;
        fillApplied    = true;
        glutPostRedisplay();
    }
}

// ─────────────────────────────────────────────────────────────
// Menu callbacks
// ─────────────────────────────────────────────────────────────
void menuShape(int value) {
    currentShape  = value;
    fillApplied   = false;  // Old fill coordinates belong to the previous shape.
    colorChosen   = false;  // Require the user to re-pick a colour for the new shape.
    glutPostRedisplay();
}

void menuColor(int value) {
    switch (value) {
        case 1: F_Color = {0,   0,   255}; break; // Blue
        case 2: F_Color = {255, 0,   0  }; break; // Red
        case 3: F_Color = {255, 255, 0  }; break; // Yellow
    }

    if (currentShape == NONE) return;

    // Apply fill immediately when a color is chosen from the menu.
    // If the user has already left-clicked a point inside the shape, keep
    // that point; otherwise default to the shape center (all shapes are
    // drawn centered at w/2, h/2).
    if (!fillApplied) {
        // Default to center only when it actually lands inside the shape.
        // Open shapes (Line, Minus) have < 3 vertices so PIP returns false.
        if (!pointInPolygon((float)(w / 2), (float)(h / 2))) return;
        fillStartX = w / 2;
        fillStartY = h / 2;
    }
    colorChosen    = true;
    savedFillColor = F_Color;
    fillApplied    = true;
    glutPostRedisplay();
}

// ─────────────────────────────────────────────────────────────
// Context menu creation
// ─────────────────────────────────────────────────────────────
void InitMenu() {

    // Triangle submenu
    int tamGiac = glutCreateMenu(menuShape);
    glutAddMenuEntry("Vuong can", TRI_RIGHT);
    glutAddMenuEntry("Deu", TRI_EQ);

    // Quadrilateral submenu
    int tuGiac = glutCreateMenu(menuShape);
    glutAddMenuEntry("Hinh chu nhat", RECTANGLE);
    glutAddMenuEntry("Hinh vuong", SQUARE);

    // Oval submenu
    int oval = glutCreateMenu(menuShape);
    glutAddMenuEntry("Hinh tron", CIRCLE);
    glutAddMenuEntry("Elip", ELLIPSE);

    // Regular polygon submenu
    int daGiac = glutCreateMenu(menuShape);
    glutAddMenuEntry("Ngu giac deu", PENTAGON);
    glutAddMenuEntry("Luc giac deu", HEXAGON);

    // Other shapes submenu
    int hinhKhac = glutCreateMenu(menuShape);
    glutAddMenuEntry("Mui ten", ARROW);
    glutAddMenuEntry("Ngoi sao", STAR);

    // Math symbol submenu
    int dau = glutCreateMenu(menuShape);
    glutAddMenuEntry("Cong", PLUS);
    glutAddMenuEntry("Tru", MINUS);
    glutAddMenuEntry("Nhan", MULTIPLY);
    glutAddMenuEntry("Chia (/)", DIVIDE);

    // Fill color submenu
    int toMau = glutCreateMenu(menuColor);
    glutAddMenuEntry("Xanh (Blue)", 1);
    glutAddMenuEntry("Do (Red)", 2);
    glutAddMenuEntry("Vang (Yellow)", 3);

    // Root menu
    glutCreateMenu(menuShape);
    glutAddMenuEntry("Duong thang", LINE);
    glutAddSubMenu("Tam giac", tamGiac);
    glutAddSubMenu("Tu giac", tuGiac);
    glutAddSubMenu("Oval", oval);
    glutAddSubMenu("Da giac deu", daGiac);
    glutAddSubMenu("Hinh khac", hinhKhac);
    glutAddSubMenu("Dau", dau);
    glutAddSubMenu("To mau", toMau);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// ─────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutCreateWindow("CS411 - 2D Object Coloring");

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glLineWidth(1.0f);

    // Dark charcoal background — contrasts with white outlines and all fill colors.
    // Pixel value ≈ {33, 33, 33}, which differs from B_Color {255,255,255}
    // and all F_Color options, so fill algorithms work correctly.
    glClearColor(0.13f, 0.13f, 0.13f, 1.0f);

    // Projection: origin at top-left
    // Pixel-perfect projection: map integer model coordinates to pixel CENTERS
    // in GL's rasterizer.  With gluOrtho2D(0, w, h, 0) model x=460 lands at
    // GL window position 460.0 — exactly on the boundary between pixels 459
    // and 460.  Drivers then rasterise GL_LINE_LOOP into pixel 459 while
    // Bresenham (softbuf) marks pixel 460 → fill overwrites the GL line.
    // Shifting by -0.5 makes model x=460 → GL window 460.5 (pixel center),
    // so the rasteriser reliably places the line in the same pixel as Bresenham.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-0.5, w - 0.5, h - 0.5, -0.5);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutMouseFunc(XuLyMouse);

    InitMenu();

    // Make glutMainLoop() return normally when the window is closed,
    // instead of calling exit() which can crash or leave GL state dirty.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutMainLoop();
    return 0;
}