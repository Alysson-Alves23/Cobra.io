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
#include <random>

using namespace std;

#define COLUMNS 40
#define ROWS 40
#define FPS 18




void display_callback();
void reshape_callback(int,int);
void keyboard_callback(unsigned char,int,int);
void timer_callback(int);
void init();
void checkPos();
void targetControl();
void scoreboard();
vector<vector<target>> grid(40,vector<target>(40));
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
int points = 0;
int targets = 0;
void display_callback(){
    checkPos();

    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    glColor3f(0.8, 0.4, 0.2);  // Marrom claro
    drawSnake();
    targetControl();
    *index += direction;
    scoreboard();
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

    if((key == 'w' || key == GLUT_KEY_UP )&& index != &posY ){

        index = &posY;
        direction = 1;
    }else
    if ((key == 's'|| key == GLUT_KEY_DOWN) && index != &posY){
        index = &posY;
        direction = -1;
    }
    else
    if((key == 'a'||key == GLUT_KEY_RIGHT)&& index != &posX){
        index = &posX;
        direction = -1;
    }
    else if((key == 'd'||key == GLUT_KEY_LEFT)&& index != &posX){
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
void checkPos(){
    if(posX>=40)
        posX=1;
    if(posX<0)
        posX=39;
    if(posY>=40)
        posY=0;
    if(posY<0)
        posY=39;
}
void targetControl(){
    if(!targets){
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> maxTargets(1, 5);
        targets = maxTargets(rng);
        for(int i = 0; i < targets; i++){
            target t;
            do {
                 t = createTarget();
            } while (grid[t.x][t.y].value != 0);

            grid[t.x][t.y] = t;
        }
    }

    int count = 0;
    for(const vector<target>& t : grid){
        for(const target& element : t){
            if(element.value){
                drawTarget(element);
                count++;
            }
        }
    }

    if(count == 0){
        targets = 0;
    }

    if(grid[posX][posY].value){
        points += grid[posX][posY].value;
        grid[posX][posY].value = 0;
        targets--;
    }
}
void scoreboard() {
    std::string pointsStr = "Points: " + std::to_string(points);
    glColor3f(0, 0, 0); // Define a cor do texto (branco)

    glRasterPos2f(1.0, 39.0); // Define a posição inicial para desenhar o texto

    // Itera pelos caracteres da string e desenha cada um individualmente
    for (char c : pointsStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c); // Especifica a fonte e o caractere a ser desenhado
    }
}