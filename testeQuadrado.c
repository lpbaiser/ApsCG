/* 
 * File:   testeTextura.c
 * Author: Leonardo Baiser <lpbaiser@gmail.com>
 *
 * Created on 23 de Junho de 2016, 03:56
 */

#include <GL/gl.h>
#include <GL/glut.h>
#include "testeQuadrado.h"

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	
	// Desenha um cubo
	
	glBegin(GL_QUADS);			// Face posterior
		glNormal3f(0.0, 0.0, 1.0);	// Normal da face
		glVertex3f(40.0, 40.0, 40.0);
		glVertex3f(-40.0, 40.0, 40.0);
		glVertex3f(-40.0, -40.0, 40.0);
		glVertex3f(40.0, -40.0, 40.0);
	glEnd();
	

	glBegin(GL_QUADS);			// Face frontal
		glNormal3f(0.0, 0.0, -1.0); 	// Normal da face
		glVertex3f(40.0, 40.0, -40.0);
		glVertex3f(40.0, -40.0, -40.0);
		glVertex3f(-40.0, -40.0, -40.0);
		glVertex3f(-40.0, 40.0, -40.0);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral esquerda
		glNormal3f(-1.0, 0.0, 0.0); 	// Normal da face
		glVertex3f(-40.0, 40.0, 40.0);
		glVertex3f(-40.0, 40.0, -40.0);
		glVertex3f(-40.0, -40.0, -40.0);
		glVertex3f(-40.0, -40.0, 40.0);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral direita
		glNormal3f(1.0, 0.0, 0.0);	// Normal da face
		glVertex3f(40.0, 40.0, 40.0);
		glVertex3f(40.0, -40.0, 40.0);
		glVertex3f(40.0, -40.0, -40.0);
		glVertex3f(40.0, 40.0, -40.0);
	glEnd();
	glBegin(GL_QUADS);			// Face superior
		glNormal3f(0.0, 1.0, 0.0);  	// Normal da face
		glVertex3f(-40.0, 40.0, -40.0);
		glVertex3f(-40.0, 40.0, 40.0);
		glVertex3f(40.0, 40.0, 40.0);
		glVertex3f(40.0, 40.0, -40.0);
	glEnd();
	glBegin(GL_QUADS);			// Face inferior
		glNormal3f(0.0, -1.0, 0.0); 	// Normal da face
		glVertex3f(-40.0, -40.0, -40.0);
		glVertex3f(40.0, -40.0, -40.0);
		glVertex3f(40.0, -40.0, 40.0);
		glVertex3f(-40.0, -40.0, 40.0);
	glEnd();
	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{   
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
                   // Evita a divisao por zero
                   if(h == 0) h = 1;
                           
                   // Especifica as dimensões da Viewport
                   glViewport(0, 0, w, h);

                   // Inicializa o sistema de coordenadas
                   glMatrixMode(GL_PROJECTION);
                   glLoadIdentity();

                   // Estabelece a janela de seleção (left, right, bottom, top)
                   if (w <= h) 
                           gluOrtho2D (0.0f, 250.0f, 0.0f, 250.0f*h/w);
                   else 
                           gluOrtho2D (0.0f, 250.0f*w/h, 0.0f, 250.0f);
}

// Programa Principal 
int principal(void)
{
     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(400,350);
     glutInitWindowPosition(10,10);
     glutCreateWindow("Quadrado");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     Inicializa();
     glutMainLoop();
}
