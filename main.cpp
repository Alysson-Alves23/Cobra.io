//*****************************************************
//
// ObjetoComTransformacoesHierarquicas.cpp
// Um programa OpenGL simples que abre uma janela GLUT, 
// desenha e permite interagir com um objeto que é 
// modelado através da utilização de transformações
// hierárquicas.
//
// Marcelo Cohen e Isabel H. Manssour
// Este código acompanha o livro 
// "OpenGL - Uma Abordagem Prática e Objetiva"
// 
//*****************************************************
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "game.hpp"

#define COLUMNS 40
#define ROWS 40

void display_callback();
void reshape_callback(int,int);
void init();
int main (int argc, char **v){
    glutInit(&argc, v);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("cobra.io");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    init();
    glutMainLoop();
}
void display_callback(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    glutSwapBuffers();
}
void reshape_callback(int w, int h){
    glViewport(0,0,(GLsizei)w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,COLUMNS,0,ROWS,-1,1);
    glMatrixMode(GL_MODELVIEW);
}
void init(){
    glClearColor(0,0,0,1);
    initGrid(COLUMNS,ROWS);
}
