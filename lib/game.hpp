#ifndef GAME_H
#define GAME_H
#include <GL/glut.h>
#include <random>
#include <chrono>
int gridX, gridY;
int posX= 20, posY = 20;
struct target{

    int color [3];
    int value = 0;
    int x,y;
};
struct segment{
    int x=19;
    int y=20;
    float color [3] = {0.8,0.4,0.2};
};
struct snake{
    int size = 0;
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

void drawGrid() {
    for(int i = 1; i<gridX-1;i++)
        for(int j = 1;j<gridY-1;j++)
        unit(i, j);
}
void drawSnake(segment s){

        glTranslated(0,0,0);
        glColor3f(0.8, 0.4, 0.2);
        glRectd(s.x,s.y,s.x+1,s.y+1);


}
void drawTarget(target t){
    //glRectd(20,20,21,21);

    glColor3f(t.color[0], t.color[1], t.color[2]);
    glRectd(t.x,t.y,t.x+1,t.y+1);
}

target createTarget(){
    int x = rand(0,40),y = rand(0,40);
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
int drawEnemy(){}
#endif // GAME_H
