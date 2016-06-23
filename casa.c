#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include "casa.h"

char ch = '1';

void loadObj(char *fname) {
    FILE *fp;
    int read;
    GLfloat x, y, z;
    glGenList = glGenLists(1);
    fp = fopen(fname, "r");
    if (!fp) {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(glGenList, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_POINTS);
        while (!(feof(fp))) {
            read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
            if (read == 4 && ch == 'v') {
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    fclose(fp);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, (GLfloat) w / (GLfloat) h, 0.1, 1000.0);
//    glOrtho(-25,25,-2,2,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}

void drawHouse() {
    glPushMatrix();
    glTranslatef(0, -40.00, -105);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(0.8, 0.8, 0.8);
    glRotatef(rotate, 0, 1, 0);
    glCallList(glGenList);
    glPopMatrix();
    rotate = rotate + 0.6;
    if (rotate > 360) {
        rotate = rotate - 360;
    }
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawHouse();
    glutSwapBuffers(); //swap the buffers
}