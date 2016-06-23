#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include "casa.h"

char ch = '1';

void SpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            obsX -= 0.2;
            break;
        case GLUT_KEY_RIGHT:
            obsX += 0.2;
            break;
        case GLUT_KEY_UP:
            obsY += 0.2;
            break;
        case GLUT_KEY_DOWN:
            obsY -= 0.2;
            break;
        case GLUT_KEY_HOME:
            obsZ += 0.2;
            break;
        case GLUT_KEY_END:
            obsZ -= 0.2;
            break;
    }
    glutPostRedisplay();
}

void loadObj(char *fname) {
    FILE *fp;
    int read;
    GLfloat x, y, z;
    GLint f1, f2, f3, f4;
    glGenList = glGenLists(1);
    fp = fopen(fname, "r");
    if (!fp) {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(glGenList, GL_COMPILE);
    glPushMatrix();
    {
        glBegin(GL_POINTS);
        while (!(feof(fp))) {
            //            read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
            read = fscanf(fp, "%c ", &ch);
            if (ch == 'v') {
                read = fscanf(fp, "%f %f %f", &x, &y, &z);
                glVertex3f(x, y, z);
            } else if (ch == 'f') {
                read = fscanf(fp, "%d/ %d/ %d/ %d/", &f1, &f2, &f3, &f4);
                glNormal3i(f1, f2, f3);
                printf("HEre");
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
    gluPerspective(80, (GLfloat) w / (GLfloat) h, 0.1, 1000.0);
    //    glOrtho(0, w, h, 0, 0, 50);
    glMatrixMode(GL_MODELVIEW);
}

void drawHouse() {
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(0.8, 0.8, 0.8);
    //    glRotatef(rotate, 0, 1, 0);
    glCallList(glGenList);
    glPopMatrix();
    rotate = rotate + 0.3;
    if (rotate > 360) {
        rotate = rotate - 360;
    }
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(obsX, obsY, obsZ,
            0, 0, 0,
            0, 1, 0);
    drawHouse();
    glutSwapBuffers(); //swap the buffers
}