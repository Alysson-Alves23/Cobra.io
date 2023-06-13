#ifndef GAME_H
#define GAME_H
#include <GL/glut.h>

int gridX, gridY;

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

#endif // GAME_H
