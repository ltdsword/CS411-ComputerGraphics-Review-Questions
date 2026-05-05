#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include "header/DrawShape.h" 
#include "header/helper.h"

int w = 800;
int h = 600;

std::vector<PolygonShape> polygons;
PolygonShape currentPolygon;
bool isDrawing = false;
int hoveredPolygonIndex = -1;
int selectedPolygonIndex = -1;
int polygonToSelect = -1;

float spawnX = 400.0f;
float spawnY = 300.0f;

int mainMenuID, selectMenuID;
std::vector<std::pair<float,float>> shapeVertices;

enum PredefinedShape {
    NONE = 0,
    LINE,
    TRI_RIGHT, TRI_EQ,
    RECTANGLE, SQUARE,
    CIRCLE, ELLIPSE,
    PENTAGON, HEXAGON,
    ARROW, STAR,
    PLUS, MINUS, MULTIPLY, DIVIDE
};

// ─────────────────────────────────────────────────────────────
// Shape Instantiation 
// ─────────────────────────────────────────────────────────────
void instantiateShape(int shapeType) {
    if (shapeType == NONE) return;
    
    // Call the original drawing functions which update extern shapeVertices
    switch(shapeType) {
        case LINE: DrawLine(); break;
        case TRI_RIGHT: DrawTriangleRight(); break;
        case TRI_EQ: DrawTriangleEq(); break;
        case RECTANGLE: DrawRectangle(); break;
        case SQUARE: DrawSquare(); break;
        case CIRCLE: DrawCircle(); break;
        case ELLIPSE: DrawEllipse(); break;
        case PENTAGON: DrawPentagon(); break;
        case HEXAGON: DrawHexagon(); break;
        case ARROW: DrawArrow(); break;
        case STAR: DrawStar(); break;
        case PLUS: DrawPlus(); break;
        case MINUS: DrawMinus(); break;
        case MULTIPLY: DrawMultiply(); break;
        case DIVIDE: DrawDivide(); break;
    }
    
    if (!shapeVertices.empty()) {
        PolygonShape newShape;
        for (auto& v : shapeVertices) {
            newShape.vertices.push_back({v.first, v.second});
        }
        newShape.calculateCenter();
        polygons.push_back(newShape);
    }
}

// ─────────────────────────────────────────────────────────────
// Display callback
// ─────────────────────────────────────────────────────────────
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (size_t i = 0; i < polygons.size(); i++) {
        if ((int)i == selectedPolygonIndex) {
            glColor3f(0.0f, 1.0f, 0.0f); // Selected (Green)
        } else if ((int)i == hoveredPolygonIndex) {
            glColor3f(0.5f, 0.5f, 1.0f); // Hovered (Blue)
        } else {
            glColor3f(1.0f, 1.0f, 1.0f); // Normal (White)
        }

        glPushMatrix();
        
        // Translating entire block
        glTranslatef(polygons[i].tx, polygons[i].ty, 0);

        // Optional: Translate to shape center, rotate/scale, translate back
        glTranslatef(polygons[i].cx, polygons[i].cy, 0);
        glRotatef(polygons[i].angle, 0, 0, 1);
        glScalef(polygons[i].scale, polygons[i].scale, 1);
        glTranslatef(-polygons[i].cx, -polygons[i].cy, 0);
        
        glBegin(GL_LINE_LOOP);
        for (size_t j = 0; j < polygons[i].vertices.size(); j++) {
            glVertex2f(polygons[i].vertices[j].x, polygons[i].vertices[j].y);
        }
        glEnd();
        
        glPopMatrix();
    }

    if (isDrawing && currentPolygon.vertices.size() > 0) {
        glColor3f(1.0f, 0.0f, 0.0f);
        
        glBegin(GL_LINE_STRIP);
        for (size_t j = 0; j < currentPolygon.vertices.size(); j++) {
            glVertex2f(currentPolygon.vertices[j].x, currentPolygon.vertices[j].y);
        }
        glEnd();
        
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        for (size_t j = 0; j < currentPolygon.vertices.size(); j++) {
            glVertex2f(currentPolygon.vertices[j].x, currentPolygon.vertices[j].y);
        }
        glEnd();
        glPointSize(1.0f);
    }

    glFlush();
}

void updateContextMenu() {
    glutDetachMenu(GLUT_RIGHT_BUTTON);
    if (!isDrawing) {
        if (hoveredPolygonIndex != -1) {
            glutSetMenu(selectMenuID);
        } else {
            glutSetMenu(mainMenuID);
        }
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}

// ─────────────────────────────────────────────────────────────
// Mouse callback
// ─────────────────────────────────────────────────────────────
void XuLyMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            if (isPointInAnyPolygon((float)x, (float)y)) return;

            if (!isDrawing) {
                isDrawing = true;
                currentPolygon.vertices.clear();
                updateContextMenu();
            }
            
            Point p = {(float)x, (float)y};
            currentPolygon.vertices.push_back(p);
            
            glutPostRedisplay();
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (isDrawing && currentPolygon.vertices.size() >= 2) {
                if (isPointInAnyPolygon((float)x, (float)y)) return;

                Point p = {(float)x, (float)y};
                currentPolygon.vertices.push_back(p);
                
                currentPolygon.calculateCenter();
                polygons.push_back(currentPolygon);
                currentPolygon.vertices.clear();
                isDrawing = false;
                
                updateContextMenu();
                glutPostRedisplay();
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Keyboard callback (transformations)
// ─────────────────────────────────────────────────────────────
void Keyboard(unsigned char key, int x, int y) {
    if (selectedPolygonIndex == -1 || selectedPolygonIndex >= polygons.size()) return;
    
    PolygonShape& poly = polygons[selectedPolygonIndex];
    if (key == 'L' || key == 'l') {
        poly.angle += 1.0f; // Rotate counter-clockwise
    } else if (key == 'R' || key == 'r') {
        poly.angle -= 1.0f; // Rotate clockwise
    } else if (key == '+') {
        poly.scale *= 1.1f;
    } else if (key == '-') {
        poly.scale /= 1.1f;
    }
    glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y) {
    if (selectedPolygonIndex == -1 || selectedPolygonIndex >= polygons.size()) return;
    
    PolygonShape& poly = polygons[selectedPolygonIndex];
    switch (key) {
        case GLUT_KEY_UP:    poly.ty -= 1.0f; break; 
        case GLUT_KEY_DOWN:  poly.ty += 1.0f; break;
        case GLUT_KEY_LEFT:  poly.tx -= 1.0f; break;
        case GLUT_KEY_RIGHT: poly.tx += 1.0f; break;
    }
    glutPostRedisplay();
}

// ─────────────────────────────────────────────────────────────
// Passive Motion (Hover effect -> Change Context Menu)
// ─────────────────────────────────────────────────────────────
void PassiveMotion(int x, int y) {
    spawnX = (float)x;
    spawnY = (float)y;

    int hover = -1;
    for (int i = (int)polygons.size() - 1; i >= 0; i--) {
        float actualX = (float)x - polygons[i].tx;
        float actualY = (float)y - polygons[i].ty;
        
        if (pointInPolygon(actualX, actualY, polygons[i].vertices)) {
            hover = i;
            break;
        }
    }

    if (hover != hoveredPolygonIndex) {
        hoveredPolygonIndex = hover;
        updateContextMenu();
        glutPostRedisplay();
    }
}

// ─────────────────────────────────────────────────────────────
// Menu Status callback
// ─────────────────────────────────────────────────────────────
void MenuStatus(int status, int x, int y) {
    if (status == GLUT_MENU_IN_USE) {
        // NEW FIX: Save the index of the shape we hovered over when the menu OPENED.
        // We use this so moving the mouse to click the menu doesn't clear the selection.
        polygonToSelect = hoveredPolygonIndex;
    } 
    else if (status == GLUT_MENU_NOT_IN_USE) {
        // Update the hover state accurately exactly when the menu closes, 
        // without calling PassiveMotion (which would wrongly change spawnX/Y to the menu click pos)
        int hover = -1;
        for (int i = (int)polygons.size() - 1; i >= 0; i--) {
            float actualX = (float)x - polygons[i].tx;
            float actualY = (float)y - polygons[i].ty;
            if (pointInPolygon(actualX, actualY, polygons[i].vertices)) {
                hover = i;
                break;
            }
        }

        if (hover != hoveredPolygonIndex) {
            hoveredPolygonIndex = hover;
            updateContextMenu();
            glutPostRedisplay();
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Context menu callback
// ─────────────────────────────────────────────────────────────
void menuAction(int value) {
    if (value == 999) {
        // NEW FIX: Evaluate against polygonToSelect instead of hoveredPolygonIndex
        if (polygonToSelect != -1) {
            selectedPolygonIndex = polygonToSelect;
        }
    } else {
        instantiateShape(value);
    }
    glutPostRedisplay();
}

void InitMenu() {
    int tamGiac = glutCreateMenu(menuAction);
    glutAddMenuEntry("Vuong can", TRI_RIGHT);
    glutAddMenuEntry("Deu", TRI_EQ);

    int tuGiac = glutCreateMenu(menuAction);
    glutAddMenuEntry("Hinh chu nhat", RECTANGLE);
    glutAddMenuEntry("Hinh vuong", SQUARE);

    int oval = glutCreateMenu(menuAction);
    glutAddMenuEntry("Hinh tron", CIRCLE);
    glutAddMenuEntry("Elip", ELLIPSE);

    int daGiac = glutCreateMenu(menuAction);
    glutAddMenuEntry("Ngu giac deu", PENTAGON);
    glutAddMenuEntry("Luc giac deu", HEXAGON);

    int hinhKhac = glutCreateMenu(menuAction);
    glutAddMenuEntry("Mui ten", ARROW);
    glutAddMenuEntry("Ngoi sao", STAR);

    int dau = glutCreateMenu(menuAction);
    glutAddMenuEntry("Cong", PLUS);
    glutAddMenuEntry("Tru", MINUS);
    glutAddMenuEntry("Nhan", MULTIPLY);
    glutAddMenuEntry("Chia", DIVIDE);

    mainMenuID = glutCreateMenu(menuAction);
    glutAddSubMenu("Tam giac", tamGiac);
    glutAddSubMenu("Tu giac", tuGiac);
    glutAddSubMenu("Oval", oval);
    glutAddSubMenu("Da giac deu", daGiac);
    glutAddSubMenu("Hinh khac", hinhKhac);
    glutAddSubMenu("Dau", dau);

    selectMenuID = glutCreateMenu(menuAction);
    glutAddMenuEntry("Select this shape", 999);

    glutSetMenu(mainMenuID);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// ─────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutCreateWindow("CS411 - Lab 03 Affine Transform 2D");

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glLineWidth(1.0f);

    glClearColor(0.13f, 0.13f, 0.13f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-0.5, w - 0.5, h - 0.5, -0.5);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutMouseFunc(XuLyMouse);
    glutPassiveMotionFunc(PassiveMotion);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);
    
    glutMenuStatusFunc(MenuStatus); 

    InitMenu();

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();
    
    return 0;
}