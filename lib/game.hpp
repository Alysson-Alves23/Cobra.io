#ifndef GAME_H
#define GAME_H
#include <GL/glut.h>
#include <random>
#include <chrono>
#include <stdio.h>
int gridX, gridY;
int posX= 20, posY = 20;
struct target{

    int color [3];
    int value = 0;
    int x,y;
};
struct segment{
    int x=0;
    int y=0;
    float color [3] = {0.8,0.4,0.2};
};
struct snake{
    int size = 0;
    std::vector<target> targets=std::vector<target>(10);
    std::vector<segment> body= std::vector<segment> (400);
};
int rand(int a,int b){

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::mt19937 rng(seed);

    std::uniform_int_distribution<int> dist(a, b);

    return dist(rng);
}
void initGrid(int columns, int rows) {
    gridX = columns;
    gridY = rows;
}

void unit(int x, int y) {
    glLineWidth(1);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + 1, y);
    glVertex2f(x + 1, y + 1);
    glVertex2f(x, y + 1);
    glEnd();
}

void drawGrid(std::vector<std::vector<int>> grid) {
    for(int i = 0; i<gridX-1;i++)
        for(int j = 0;j<gridY-1;j++){

        unit(i, j);
            glColor3f(0,0,0);
            glRasterPos2f(i, j); // Define a posicao inicial para desenhar o texto


            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, std::to_string(grid[i][j])[0]); // Especifica a fonte e o caractere a ser desenhado
            //printf(std::to_string(grid[i][j]).c_str());
        }
}
void drawSnake(segment s){

        glTranslated(0,0,0);

        glRectd(s.x,s.y,s.x+1,s.y+1);


}
void drawTarget(target t){
    //glRectd(20,20,21,21);

    glColor3f(t.color[0], t.color[1], t.color[2]);
    glRectd(t.x,t.y,t.x+1,t.y+1);
}

target createTarget(){
    int x = rand(0,39),y = rand(0,39);
    target t;
    t.x=x;
    t.y=y;
    t.value = rand(1,3)*5;
    switch (t.value) {
        case 15:
            t.color[0] = 1;
            t.color[1] = 0;
            t.color[2] = 0;
            break;
        case 10:
            t.color[0] = 1;
            t.color[1] = 0.5;
            t.color[2] = 0;
            break;
        case 5:
            t.color[0] = 1;
            t.color[1] = 1;
            t.color[2] = 0;
            break;
    }
    return t;
}
snake drawEnemy(){
    snake enemy;
    enemy.body[0].y = 30;
    enemy.body[0].x = 30;

    enemy.size = rand(2,6);
    return  enemy;
}
#endif // GAME_H
