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
#define FPS 15

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
void enemyController();
pair<int,int> foodGPS(snake);
vector<vector<target>> targetGrid(40,vector<target>(40));

vector<vector<int>> grid(40,vector<int>(40));
vector<snake> enemies;
int* index = &posX;
int direction = 1;
int points = 0;
int targets = 0;
int vidas = 3;
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
        vidas = 3;
        points = 0;
        posX= 20;
        posY= 20;
        glutDisplayFunc(display_callback);

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
    enemies.push_back(drawEnemy());
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

void display_callback(){

    checkPos();
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid(grid);

    // moveSnake();

    enemyController();
    targetControl();
   // *index += direction;
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
	vidas--;
    for(segment seg : s.body)
        grid[seg.x][seg.y]=0;
	printf("Vidas restantes:%d\n",vidas);
    printf("x:%d\n",posX);
    printf("y:%d\n",posY);
    posX =20;
    posY= 20;
    s.size= 1;
    if(vidas<=0)
        glutDisplayFunc(gameover);
    
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

                 s.targets.push_back(t);

                 for( segment b: s.body){
                     if(t.x== b.x && t.y == b.y){
                         t.value =0;
                     }
                 }
                 for(snake enemy: enemies)
                     for( segment b: enemy.body){
                         if(t.x== b.x && t.y == b.y){
                             t.value =0;
                         }
                     }

                 targetGrid[t.x][t.y] = t;
            } while (targetGrid[t.x][t.y].value == 0 );


        }
    }

    int count = 0;
    for(const vector<target>& t : targetGrid){
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

    if(targetGrid[posX][posY].value){
        segment b;
        bool horizontal = index == &posX;
        b.x = s.body[s.body.size()-1].x -(horizontal?1:0);
        b.y = s.body[s.body.size()-1].y - (horizontal?0:1);
        s.size+=targetGrid[posX][posY].value/5;
        printf("size : %d\n",s.size);
       // s.body.resize(s.size);
        //s.body.size(s);


            for(int i = 0;i < s.targets.size(); i++){
                if(s.targets[i].y == posX &&s.targets[i].y == posY){
                    printf("alvo apagado! em %d %d", s.targets[i].y,s.targets[i].x);
                    s.targets.erase(s.targets.begin()+i);
                    s.targets[i]={};
                    s.targets[i]={};
                break;
                }
                for(snake &enemy: enemies) {
                    enemy.targets = s.targets;
                }
        }
        points += targetGrid[posX][posY].value;
        targetGrid[posX][posY].value = 0;
        targets--;
    }
}
void moveSnake(){

    int lastX = s.body[0].x,lastY = s.body[0].y;
    grid[lastX][lastY] = 1;

    for(int i = 0; i< s.size+1;i++) {
           int auxX=s.body[i].x, auxY=s.body[i].y;
           s.body[i].x = lastX;
           s.body[i].y = lastY;

           lastX= auxX;
           lastY=auxY;
      //
    }
    s.body[0].x = posX;
    s.body[0].y = posY;
    grid[lastX][lastY] = 0;
    for(int i = 0; i< s.size;i++)
        if(grid[s.body[0].x][s.body[0].y]){
            kill();
        } else{
            glColor3f(0.8, 0.4, 0.2);
            drawSnake(s.body[i]);}

}
void scoreboard() {
    std::string pointsStr = "Points: " + std::to_string(points) + "  Vidas: " + std::to_string(vidas) ;
    
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
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
    grid = vector<vector<int>>(40,vector<int>(40));
	glClear(GL_COLOR_BUFFER_BIT);
    points = 0;
	glLineWidth(2);
	glRasterPos2f(16,20); 
	glColor3f(1,0,0);
	DesenhaTexto(GLUT_BITMAP_9_BY_15,"Game Over");
	
	glutSwapBuffers();
	
}
int count = 10;
void enemyController(){
    for(snake &enemy : enemies){
        pair<int,int> tgs = foodGPS(enemy);
        pair<int,int> minFit = {-1,-1};
        pair<int,int> simu[4] = {{enemy.body[0].x-1,enemy.body[0].y},{enemy.body[0].x+1,enemy.body[0].y},
                                 {enemy.body[0].x,enemy.body[0].y-1},{enemy.body[0].x,enemy.body[0].y+1}};
        for(int i = 0; i< 4; i++){

            int flag= 1;
            for(int j = 0; j< enemy.size;j++){
                if( simu[i].first>= 40 || simu[i].first< 0  ||simu[i].second>= 40 || simu[i].second< 0){
                    flag=0;
                    break;
                }
                if((grid[simu[i].first][simu[i].second])){

                    ::printf("\ncolide a parte %d %d coord %d %d valor  %d",i,j,enemy.body[j+1].x,enemy.body[j+1].y,grid[simu[i].first][simu[i].second] );
                    flag = 0;
                    break;
                }
            }
            if(flag)
                if( minFit != pair<int,int>(-1,-1)){
                    int simuFit = abs(abs(simu[i].first - tgs.first) + abs(tgs.second- simu[i].second) );
                    int minSimuFit = abs( abs(minFit.first  - tgs.first) + abs(minFit.second-tgs.second) );
                    ::printf("\n%d < %d\n",simuFit,minFit);
                    if(simuFit< minSimuFit)
                       minFit = simu[i];
                    else
                        if(simuFit == minSimuFit ) {

                            pair<int, int> sortear[2] = {minFit, simu[i]};
                            minFit = sortear[(rand(0, 100) % 2) > 0];
                        }
                    ::printf("tgs = {%d,%d}\n",tgs.first,tgs.second);
                        ::printf("minFitCord = {%d,%d}\n",minFit.first,minFit.second);
                    ::printf("posCord = {%d,%d}\n",simu[i].first,simu[i].second);

                }else
                    minFit= simu[i];


        }

        ::printf("minFit = {%d,%d}",minFit.first,minFit.second);
        int lastX = enemy.body[0].x,lastY = enemy.body[0].y;
        grid[lastX][lastY] = 1;
        if(minFit == pair<int,int>(-1,-1)){
            for(segment seg : enemy.body)
                grid[seg.x][seg.y]=0;
            enemy.size= 2;
            enemy.body[0].x = 30;
            enemy.body[0].y = 30;
        return;
        }

        for(int i = 1; i< enemy.size;i++) {
            int auxX=enemy.body[i].x, auxY=enemy.body[i].y;
            enemy.body[i].x = lastX;
            enemy.body[i].y = lastY;
            lastX= auxX;
            lastY= auxY;
            //
        }
        grid[lastX][lastY] = 0;
        enemy.body[0].x = minFit.first;
        enemy.body[0].y = minFit.second;
        for(int i = 0; i< enemy.size;i++) {

            glColor3f(0.5, 0.5, 1);
            drawSnake(enemy.body[i]);
        }
        if(targetGrid[enemy.body[0].x][enemy.body[0].y].value){
            enemy.size+=targetGrid[enemy.body[0].x][enemy.body[0].y].value/5;
            points += targetGrid[enemy.body[0].x][enemy.body[0].y].value;
            targetGrid[enemy.body[0].x][enemy.body[0].y].value = 0;
            targets--;
        }
    }
}
pair<int,int>foodGPS(snake snake){
    pair<int,int> tgs = {-1,-1},aux ;
    for(vector<target> tg: targetGrid)
        for(target t: tg){
            if(t.value)
            if(tgs.first +tgs.second >-1 ){

                aux = {t.x,t.y};
                tgs =abs(t.x- snake.body[0].x) + abs(t.y - snake.body[0].y) <abs(tgs.first - snake.body[0].x)+abs(tgs.second - snake.body[0].y)?aux :tgs;

            }else
                tgs = {t.x,t.y};
        }
    std::string pointsStr = "x: " + std::to_string(abs(tgs.first - snake.body[0].x)) + " y: " + std::to_string(abs(tgs.second - snake.body[0].y)) ;

    glColor3f(0, 0, 0); // Define a cor do texto (sobranco)

    glRasterPos2f(1.0, 36.0); // Define a posicao inicial para desenhar o texto

    // Itera pelos caracteres da string e desenha cada um individualmente
    for (char c : pointsStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c); // Especifica a fonte e o caractere a ser desenhado
    }
    return tgs;

}