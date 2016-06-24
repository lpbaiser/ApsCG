#include <GL/glut.h>
#include "casa.h"
#include "testeQuadrado.h"

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Casa Número 0");
    glutReshapeFunc(reshape);
//    glutDisplayFunc(display);
//    glutIdleFunc(display);
    principal();
//    Inicializa();
//    loadObj("casa.obj");
    glutMainLoop();
    return 0;
}
