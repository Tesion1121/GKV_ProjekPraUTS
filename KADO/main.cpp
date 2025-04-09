#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>

// Posisi kamera
const float camX = 5.0f, camY = 5.0f, camZ = 10.0f;
const float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;

// Variabel rotasi objek
float objAngleX = 0.0f, objAngleY = 0.0f;
float rotSpeed = 5.0f;

// Deklarasi fungsi
void init();
void display();
void reshape(int w, int h);
void specialKeyboard(int key, int x, int y);
void drawBox(float scaleX, float scaleY, float scaleZ, float r, float g, float b);
void drawBow(float ringRadius, float tubeRadius, float r, float g, float b);
void drawSideRibbons(float boxWidth, float boxHeight, float boxDepth, float lidHeight, float ribbonWidth);
void drawTopPlusRibbon(float lidWidth, float lidDepth, float lidHeight);

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void drawBox(float scaleX, float scaleY, float scaleZ, float r, float g, float b) {
    glColor3f(r, g, b);
    glPushMatrix();
        glScalef(scaleX, scaleY, scaleZ);
        glutSolidCube(1.0);
    glPopMatrix();
}

void drawBow(float ringRadius, float tubeRadius, float r, float g, float b) {
    glColor3f(r, g, b);
    int numLoops = 4;
    for (int i = 0; i < numLoops; i++) {
        glPushMatrix();
            float angle = (360.0f / numLoops) * i;
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
            glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, ringRadius / 2.0f);
            glutSolidTorus(tubeRadius, ringRadius, 16, 32);
        glPopMatrix();
    }
}

void drawSideRibbons(float boxWidth, float boxHeight, float boxDepth, float lidHeight, float ribbonWidth) {
    float totalHeight = boxHeight + lidHeight;
    float yCenter = totalHeight * 0.5f;
    float halfW = boxWidth * 0.5f;
    float halfD = boxDepth * 0.5f;
    float thickness = 0.02f;

    glColor3f(1.0f, 1.0f, 0.0f);

    // Pita depan
    glPushMatrix();
        glTranslatef(0.0f, yCenter, halfD + thickness);
        glScalef(ribbonWidth, totalHeight, thickness);
        glutSolidCube(1.0);
    glPopMatrix();

    // Pita belakang
    glPushMatrix();
        glTranslatef(0.0f, yCenter, -halfD - thickness);
        glScalef(ribbonWidth, totalHeight, thickness);
        glutSolidCube(1.0);
    glPopMatrix();

    // Pita kiri
    glPushMatrix();
        glTranslatef(-halfW - thickness, yCenter, 0.0f);
        glScalef(thickness, totalHeight, ribbonWidth);
        glutSolidCube(1.0);
    glPopMatrix();

    // Pita kanan
    glPushMatrix();
        glTranslatef(halfW + thickness, yCenter, 0.0f);
        glScalef(thickness, totalHeight, ribbonWidth);
        glutSolidCube(1.0);
    glPopMatrix();
}

void drawTopPlusRibbon(float lidWidth, float lidDepth, float lidHeight) {
    float thickness = 0.02f;
    glColor3f(1.0f, 1.0f, 0.0f);
    
    // Horizontal
    glPushMatrix();
        glTranslatef(0.0f, lidHeight * 0.5f, 0.0f);
        glScalef(lidWidth, thickness, 0.3f);
        glutSolidCube(1.0);
    glPopMatrix();
    
    // Vertikal
    glPushMatrix();
        glTranslatef(0.0f, lidHeight * 0.5f, 0.0f);
        glScalef(0.3f, thickness, lidDepth);
        glutSolidCube(1.0);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

    glPushMatrix();
        glRotatef(objAngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(objAngleY, 0.0f, 1.0f, 0.0f);

        float boxW = 2.0f, boxH = 1.4f, boxD = 2.0f, lidH = 0.6f;

        // Badan kado
        glPushMatrix();
            glTranslatef(0.0f, boxH * 0.5f, 0.0f);
            drawBox(boxW, boxH, boxD, 1.0f, 0.3f, 0.5f);
        glPopMatrix();

        // Pita atas di bawah tutup kado
        glPushMatrix();
            glTranslatef(0.0f, 0.01f, 0.0f);
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
            drawTopPlusRibbon(2.05f, 2.05f, 0.1f);
        glPopMatrix();

        // Tutup kado dan dekorasi
        glPushMatrix();
            glTranslatef(0.0f, boxH + lidH * 0.5f, 0.0f);
            drawBox(2.05f, lidH, 2.05f, 1.0f, 0.2f, 0.4f);
            drawTopPlusRibbon(2.05f, 2.05f, lidH);

            // Dekorasi cone hijau
            glPushMatrix();
                glTranslatef(0.0f, lidH * 0.5f + 0.15f, 0.0f);
                glColor3f(0.2f, 0.8f, 0.3f);
                glutSolidCone(0.2, 0.4, 16, 16);
            glPopMatrix();
        glPopMatrix();

        // Pita samping kado
        drawSideRibbons(boxW, boxH, boxD, lidH, 0.3f);

        // Pita loop (bow) di atas kado
        glPushMatrix();
            glTranslatef(0.0f, boxH + lidH + 0.05f, 0.0f);
            drawBow(0.3f, 0.07f, 1.0f, 1.0f, 0.0f);
        glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void specialKeyboard(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:    objAngleX -= rotSpeed; break;
        case GLUT_KEY_DOWN:  objAngleX += rotSpeed; break;
        case GLUT_KEY_LEFT:  objAngleY -= rotSpeed; break;
        case GLUT_KEY_RIGHT: objAngleY += rotSpeed; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Kado 3D");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard);
    glutMainLoop();
    return 0;
}

