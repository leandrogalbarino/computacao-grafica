/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         02/2025
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.1
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "bmp.h"
#include "checkbox.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include "Camada.h"
#include "LayerManager.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1950, screenHeight = 1080;

// int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Menu *menuSide;
MenuLayer *menuTop;
MenuLayer *menuLayer;

void menusCreate(){
    menuSide = new Menu(0,0, 50,1200, 10);
    menuTop = new MenuLayer(51,0, 1200,50, 10);
    menuLayer = new MenuLayer(1201, 0, 1250, 1200 , 3);
}
void menusCollision(int x, int y){
    menuTop->collisionButtons(x,y);
    menuSide->collisionButtons(x,y);
    menuLayer->collisionButtons(x,y);
}
void menusSetCoord(int x, int y, int _x, int _y){
    menuTop->setCoord(x,y, _x, _y);
    menuSide->setCoord(x,y, _x, _y);
    menuLayer->setCoord(x,y, _x, _y);
}

// Camada *camada1;
// LayerManager *layerManager;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.






void render()
{
    // camada1->img->render();
    menuSide->render();
    menuTop->render();
    menuLayer->render();
    // layerManager->drawLayers();
    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}





//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int _x, int _y)
{
    static int x = 0;  // Variável estática mantém valor entre chamadas da função
    static int y = 0;
    if(state == 0){
        menusCollision(_x, _y);
        x = _x;
        y = _y;
    }
    if(state == 1){
        menusSetCoord(x, y, _x, _y);
   }
}



int main(void)
{
    menusCreate();
    CV::init(&screenWidth, &screenHeight, "Leandro Galbarino");
    CV::run();
}
