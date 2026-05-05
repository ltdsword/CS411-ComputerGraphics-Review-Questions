#include <windows.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../include/SOIL/SOIL.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global animation angle
GLfloat angle = 0.0f;

// Textures
GLuint textures[8];
// Expanded random indices buffer to fully support independent textures per EVERY surface (Phase 4 requirement)
int objTexIndex[30];

// Pointer for GLU quadric objects
GLUquadric* quadric;

void drawTexturedCube(float size, int* texIndices) {
    float h = size / 2.0f;
    
    // Front Face
    glBindTexture(GL_TEXTURE_2D, textures[texIndices[0]]);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-h, -h,  h);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( h, -h,  h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( h,  h,  h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-h,  h,  h);
    glEnd();
    
    // Back Face
    glBindTexture(GL_TEXTURE_2D, textures[texIndices[1]]);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, -h, -h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-h,  h, -h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( h,  h, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( h, -h, -h);
    glEnd();
    
    // Top Face
    glBindTexture(GL_TEXTURE_2D, textures[texIndices[2]]);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-h,  h, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-h,  h,  h);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( h,  h,  h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( h,  h, -h);
    glEnd();
    
    // Bottom Face
    glBindTexture(GL_TEXTURE_2D, textures[texIndices[3]]);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-h, -h, -h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( h, -h, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( h, -h,  h);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, -h,  h);
    glEnd();
    
    // Right face
    glBindTexture(GL_TEXTURE_2D, textures[texIndices[4]]);
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( h, -h, -h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( h,  h, -h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( h,  h,  h);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( h, -h,  h);
    glEnd();
    
    // Left Face
    glBindTexture(GL_TEXTURE_2D, textures[texIndices[5]]);
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-h, -h, -h);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, -h,  h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-h,  h,  h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-h,  h, -h);
    glEnd();
}

void drawHyperboloid(float baseRadius, float waistRadius, float height, int slices, int stacks) {
    float halfHeight = height / 2.0f;
    float coef = ((baseRadius * baseRadius) / (waistRadius * waistRadius) - 1.0f) / (halfHeight * halfHeight);
    
    for (int i = 0; i < stacks; i++) {
        float y1 = -halfHeight + (height * i) / stacks;
        float y2 = -halfHeight + (height * (i + 1)) / stacks;
        
        float r1 = waistRadius * sqrt(1.0f + coef * y1 * y1);
        float r2 = waistRadius * sqrt(1.0f + coef * y2 * y2);
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float theta = (2.0f * M_PI * j) / slices;
            float cosT = cos(theta);
            float sinT = sin(theta);
            
            float nx1 = r1 * cosT;
            float nz1 = r1 * sinT;
            float ny1 = -waistRadius * waistRadius * coef * y1;
            float len1 = sqrt(nx1*nx1 + ny1*ny1 + nz1*nz1);
            if(len1 > 0.0f) { nx1/=len1; ny1/=len1; nz1/=len1; }
            
            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f((float)j/slices, (float)i/stacks);
            glVertex3f(r1 * cosT, y1, r1 * sinT);
            
            float nx2 = r2 * cosT;
            float nz2 = r2 * sinT;
            float ny2 = -waistRadius * waistRadius * coef * y2;
            float len2 = sqrt(nx2*nx2 + ny2*ny2 + nz2*nz2);
            if(len2 > 0.0f) { nx2/=len2; ny2/=len2; nz2/=len2; }
            
            glNormal3f(nx2, ny2, nz2);
            glTexCoord2f((float)j/slices, (float)(i+1)/stacks);
            glVertex3f(r2 * cosT, y2, r2 * sinT);
        }
        glEnd();
    }
}

void drawParaboloid(float baseRadius, float height, int slices, int stacks) {
    for (int i = 0; i < stacks; i++) {
        float y1 = (height * i) / stacks;
        float y2 = (height * (i + 1)) / stacks;
        
        float r1 = baseRadius * sqrt(1.0f - y1 / height);
        float r2 = baseRadius * sqrt(1.0f - y2 / height);
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float theta = (2.0f * M_PI * j) / slices;
            float cosT = cos(theta);
            float sinT = sin(theta);
            
            float nx1 = 2.0f * height / (baseRadius * baseRadius) * (r1 * cosT);
            float nz1 = 2.0f * height / (baseRadius * baseRadius) * (r1 * sinT);
            float ny1 = 1.0f;
            float len1 = sqrt(nx1*nx1 + ny1*ny1 + nz1*nz1);
            if(len1 > 0.0f) { nx1/=len1; ny1/=len1; nz1/=len1; }
            
            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f((float)j/slices, (float)i/stacks);
            glVertex3f(r1 * cosT, y1, r1 * sinT);
            
            float nx2 = 2.0f * height / (baseRadius * baseRadius) * (r2 * cosT);
            float nz2 = 2.0f * height / (baseRadius * baseRadius) * (r2 * sinT);
            float ny2 = 1.0f;
            float len2 = sqrt(nx2*nx2 + ny2*ny2 + nz2*nz2);
            if(len2 > 0.0f) { nx2/=len2; ny2/=len2; nz2/=len2; }
            
            glNormal3f(nx2, ny2, nz2);
            glTexCoord2f((float)j/slices, (float)(i+1)/stacks);
            glVertex3f(r2 * cosT, y2, r2 * sinT);
        }
        glEnd();
    }
}

void initGL() {
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f); 
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    const char* imageFiles[] = {
        "data/00.bmp", "data/01.bmp", "data/02.bmp", "data/03.bmp",
        "data/04.bmp", "data/05.bmp", "data/06.bmp", "data/07.bmp"
    };

    for(int i = 0; i < 8; i++) {
        textures[i] = SOIL_load_OGL_texture(
            imageFiles[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y
        );

        if (textures[i] == 0) {
            printf("SOIL loading error for %s: '%s'\n", imageFiles[i], SOIL_last_result());
        } else {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    srand((unsigned int)time(NULL));
    for(int i = 0; i < 30; i++) {
        objTexIndex[i] = rand() % 8;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Explicitly reset color to white so textures map with absolute true colors
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move camera back and angle downwards securely
    glTranslatef(0.0f, -0.5f, -11.5f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f); 

    // ================= TOP ROW =================
    
    // Top Left: Cube (Phase 3b strict manual texture vertices on ALL 6 surfaces independently)
    glPushMatrix();
        glTranslatef(-4.5f, 2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f); 
        // Force all faces of the cube to use texture index 0
        int cubeTex[6] = {0, 0, 0, 0, 0, 0};
        drawTexturedCube(1.5f, cubeTex);
    glPopMatrix();

    // Top Middle-Left: Sphere (No caps needed)
    glPushMatrix();
        glTranslatef(-1.5f, 2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        gluSphere(quadric, 1.0, 32, 32);
    glPopMatrix();

    // Top Middle-Right: Cylinder (Solid with Top & Bottom caps dynamically textured)
    glPushMatrix();
        glTranslatef(1.5f, 2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glTranslatef(0.0f, 0.0f, -1.0f);
        
        // Tube
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        gluCylinder(quadric, 1.0, 1.0, 2.0, 32, 32);
        // Base Cap
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f); // Prevents inverse lighting normals
            gluDisk(quadric, 0.0, 1.0, 32, 1);
        glPopMatrix();
        // Top Cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 2.0f);
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            gluDisk(quadric, 0.0, 1.0, 32, 1);
        glPopMatrix();
    glPopMatrix();

    // Top Right: Cone (With Bottom Cap)
    glPushMatrix();
        glTranslatef(4.5f, 2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glTranslatef(0.0f, 0.0f, -1.0f);
        
        // Base Face
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        gluCylinder(quadric, 1.0, 0.0, 2.0, 32, 32);
        // Cap
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, textures[3]);
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
            gluDisk(quadric, 0.0, 1.0, 32, 1);
        glPopMatrix();
    glPopMatrix();

    // ================= BOTTOM ROW =================
    
    // Bottom Left: Flat Disk (With Caps dynamically textured)
    glPushMatrix();
        glTranslatef(-4.5f, -2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glTranslatef(0.0f, 0.0f, -0.1f);
        
        // Outer rim
        glBindTexture(GL_TEXTURE_2D, textures[4]);
        gluCylinder(quadric, 1.2, 1.2, 0.2, 32, 32);
        // Bottom Base Cap
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, textures[4]);
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
            gluDisk(quadric, 0.0, 1.2, 32, 1);
        glPopMatrix();
        // Top Cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.2f);
            glBindTexture(GL_TEXTURE_2D, textures[4]);
            gluDisk(quadric, 0.0, 1.2, 32, 1);
        glPopMatrix();
    glPopMatrix();

    // Bottom Middle-Left: Torus (Textured via GEN_S/T)
    glPushMatrix();
        glTranslatef(-1.5f, -2.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glRotatef(-angle, 0.0f, 0.0f, 1.0f); // Rotate around the torus's ACTUAL central axis (local Z after tilt)

        glBindTexture(GL_TEXTURE_2D, textures[5]);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        
        // Use OBJECT_LINEAR to ensure the texture "sticks" to the rotating ring surface
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        
        glutSolidTorus(0.35, 0.8, 32, 32);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    glPopMatrix();

    // Bottom Middle-Right: True Hyperboloid (With specific Capping)
    glPushMatrix();
        glTranslatef(1.5f, -2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
        
        glBindTexture(GL_TEXTURE_2D, textures[6]);
        drawHyperboloid(1.0f, 0.3f, 2.0f, 32, 32); 
        
        // Top Cap  (Local Y = 1.0)
        glPushMatrix();
            glTranslatef(0.0f, 1.0f, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Tip normal up to +Y
            glBindTexture(GL_TEXTURE_2D, textures[6]);
            gluDisk(quadric, 0.0, 1.0, 32, 1);
        glPopMatrix();
        // Bottom Cap (Local Y = -1.0)
        glPushMatrix();
            glTranslatef(0.0f, -1.0f, 0.0f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Tip normal down to -Y
            glBindTexture(GL_TEXTURE_2D, textures[6]);
            gluDisk(quadric, 0.0, 1.0, 32, 1);
        glPopMatrix();
    glPopMatrix();

    // Bottom Right: True Paraboloid (With specific Base Capping)
    glPushMatrix();
        glTranslatef(4.5f, -2.0f, 0.0f);
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -0.6f, 0.0f); 
        
        glBindTexture(GL_TEXTURE_2D, textures[7]);
        drawParaboloid(1.2f, 1.2f, 32, 32); 
        
        // Base Cap (Local Y = 0)
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, textures[7]);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Point -Z downward to -Y
            gluDisk(quadric, 0.0, 1.2, 32, 1);
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void timer(int value) {
    angle += 1.0f; // Clockwise logic controlled by negative assignment in display()
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(15, timer, 0); // Animation interval (~60 FPS mapping Phase 6)
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS411 - Lab 05 - Grid Layout Solid Faces");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0); // Activate Timer Hook

    glutMainLoop();
    return 0;
}
