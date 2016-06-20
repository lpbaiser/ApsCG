#pragma once

#include <GL/gl.h>

GLuint elephant;
float elephantrot;

/**
 * Carrega objeto.
 * @param fname
 */
void loadObj(char *fname);
void reshape(int w, int h);
void drawCar();
void display();