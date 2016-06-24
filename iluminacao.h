#pragma once

#include <GL/gl.h>

GLuint glGenList;


/* 
 * File:   inluminacao.c
 * Author: Leonardo Baiser <lpbaiser@gmail.com>
 *
 * Created on 23 de Junho de 2016, 20:39
 */


void Desenha(void);
void Inicializa (void);
void EspecificaParametrosVisualizacao(void);
void AlteraTamanhoJanela(GLsizei w, GLsizei h);
void GerenciaMouse(int button, int state, int x, int y);
int principal(void);
