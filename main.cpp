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
#define FPS 18
void display_callback();
void reshape_callback(int,int);
void keyboard_callback(unsigned char,int,int);
void timer_callback(int);
void init();
int main (int argc, char **v){

    glutInit(&argc, v);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("cobra.io");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);
    glutTimerFunc(0,timer_callback,0);
    init();
    glutMainLoop();
}
int* index = &posX;
int direction = 1;
void display_callback(){
    if(*index>=40)
        *index=0;
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    glColor3f(0.8, 0.4, 0.2);  // Marrom claro
    drawSnake();
    *index += direction;
    glutSwapBuffers();
}
void reshape_callback(int w, int h){
    glViewport(0,0,(GLsizei)w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,COLUMNS,0,ROWS,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard_callback(unsigned  char key, int ,int ){
    if(key == 'w' || key == 77){
        index = &posY;
        direction = 1;
    }else
    if (key == 's'|| key == 80){
        index = &posY;
        direction = -1;
    }
    else
    if(key == 'a'||key == 75){
        index = &posX;
        direction = -1;
    }
    else if(key == 'd'||key == 77){
        index = &posX;
        direction = 1;
    }
}
void timer_callback(int){
    glutPostRedisplay();
    glutTimerFunc(1000/FPS,timer_callback,0);
}
void init(){
    glClearColor(1,1,1,1);
    initGrid(COLUMNS,ROWS);
}
