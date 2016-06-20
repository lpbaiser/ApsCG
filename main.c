#include <GL/glut.h>
#include "car.h"

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("ObjLoader");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    // loadObj("monkey.obj");
    // loadObj("Bugatti-Veyron.obj");
    loadObj("robot2.obj");
    glutMainLoop();
    return 0;
}
