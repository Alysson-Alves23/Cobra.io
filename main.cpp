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

snake s;

void display_callback();
void reshape_callback(int,int);
void keyboard_callback(unsigned char,int,int);
void timer_callback(int);
void init();
void checkPos();
void targetControl();
void moveSnake();
void scoreboard();
void kill();
void gameover();

vector<vector<target>> grid(40,vector<target>(40));

void MenuInformacoes(int op)
{
   switch(op) {
            case 2:
            		printf("Como jogar:\nW para cima\nS para baixo\nA para direita\nD para esquerda\n");
			        break;
    glutPostRedisplay();
	}
}

// Gerenciamento do menu principal
void MenuPrincipal(int op)
{
    if (op == 0) {
            exit(0);
    }else if(op == 1){
    	//fazer com o que o jogo volte a rodar 
	}
}

void CriaMenu()
{
    int menu,submenu1,submenu2;

	submenu1 = glutCreateMenu(MenuInformacoes);
	glutAddMenuEntry("W para cima",2);
	glutAddMenuEntry("A para esquerda",2);
	glutAddMenuEntry("S para baixo ",2);
	glutAddMenuEntry("D para direita",2);

    menu = glutCreateMenu(MenuPrincipal);

	glutAddMenuEntry("Jogar",1);
	
    glutAddSubMenu("Como Jogar",submenu1);
    
    glutAddMenuEntry("Sair",0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Fun��o callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state)
{
    if (button == GLUT_RIGHT_BUTTON)
         if (state == GLUT_DOWN)
            CriaMenu();

    glutPostRedisplay();
}

int main (int argc, char **v){

    segment b;
    b.x = posX;
    b.y = posY;
    s.body.push_back(b);
    s.size++;
    glutInit(&argc, v);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("cobra.io");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);
    glutTimerFunc(0,timer_callback,0);
	CriaMenu();
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
     // Marrom claro

    moveSnake();


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
void kill(){
	glutDisplayFunc(gameover);  
    posX =20;
    posY= 20;
    s.size= 1;
    points = 0;
    
}
void checkPos(){
    if(posX >= 40 || posY>= 40 || posX<0 || posY< 0)
        kill();
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
                 for( segment b: s.body){
                     if(t.x== b.x && t.y == b.y){
                         t.value =0;
                     }
                 }
            } while (grid[t.x][t.y].value != 0 );

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
        segment b;
        bool horizontal = index == &posX;
        b.x = s.body[s.body.size()-1].x -(horizontal?1:0);
        b.y = s.body[s.body.size()-1].y - (horizontal?0:1);
        s.size+=grid[posX][posY].value/5;
        printf("size : %d\n",s.size);
       // s.body.resize(s.size);
        //s.body.size(s);


        points += grid[posX][posY].value;
        grid[posX][posY].value = 0;
        targets--;
    }
}
void moveSnake(){

    int lastX = s.body[0].x,lastY = s.body[0].y;
    for(int i = 1; i< s.size;i++) {
           int auxX=s.body[i].x, auxY=s.body[i].y;
           s.body[i].x = lastX;
           s.body[i].y = lastY;
           lastX= auxX;
           lastY=auxY;
      //
    }
    s.body[0].x = posX;
    s.body[0].y = posY;
    for(int i = 0; i< s.size;i++)
        if(s.body[0].x == s.body[i+1].x  && s.body[0].y==s.body[i+1].y){
            kill();
        } else
            drawSnake(s.body[i]);

}
void scoreboard() {
    std::string pointsStr = "Points: " + std::to_string(points);
    
    glColor3f(0, 0, 0); // Define a cor do texto (sobranco)

    glRasterPos2f(1.0, 37.0); // Define a posicao inicial para desenhar o texto
    
    // Itera pelos caracteres da string e desenha cada um individualmente
    for (char c : pointsStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c); // Especifica a fonte e o caractere a ser desenhado
    }
    
   string info = "Menu abre ao clicar com botao direito na tela";

	glRasterPos2f(1.0, 39.0); // Define a posicao inicial para desenhar o texto
    
    // Itera pelos caracteres da string e desenha cada um individualmente
    for (char c : info) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c); // Especifica a fonte e o caractere a ser desenhado
    }
}
void DesenhaTexto(void *font, char *string) 
{
	// Exibe caractere a caractere
	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*string++); 
}

void gameover(){
	printf("Game Over");
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
     
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLineWidth(2);
	glRasterPos2f(16,20); 
	glColor3f(1,0,0);
	DesenhaTexto(GLUT_BITMAP_9_BY_15,"Game Over");
	
	glutSwapBuffers();
	
}
