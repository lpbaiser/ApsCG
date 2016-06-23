#pragma once

#include <GL/gl.h>

GLuint glGenList;
float rotate;

/**
 * Carrega objeto.
 * @param fname
 */
void loadObj(char *fname);
void reshape(int w, int h);
void drawHouse();
void display();