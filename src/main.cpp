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

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1950, screenHeight = 1080;

// int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

MenuFunctions *menuSide;
MenuLayer *menuLayer;

void menusCreate()
{
    menuLayer = new MenuLayer(1201, 0, 1250, 1200, 6);
    menuSide = new MenuFunctions(0, 0, 50, 1200, 10);
    menuSide->setLayerManager(menuLayer->getLayerManager());
}
void menusCollision(int x, int y)
{
    if (menuLayer->collisionButtons(x, y))
    {
        menuSide->notOperating();
    }
    else
    {
        menuSide->collisionButtons(x, y);
    }
}

void menusHandleClick(int x, int y, int _x, int _y)
{
    menuLayer->handleClick(x, y, _x, _y);
    menuSide->handleClick(x, y, _x, _y);
}

void menusRender()
{
    menuLayer->render();
    menuSide->render();
}

void render()
{
    menusRender();
    Sleep(10); // nao eh controle de FPS. Somente um limitador de FPS.
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int _x, int _y)
{
    static int x = 0; // Variável estática mantém valor entre chamadas da função
    static int y = 0;
    if (state == 0)
    {
        menusCollision(_x, _y);
        x = _x;
        y = _y;
    }
    if (state == 1)
    {
        menusHandleClick(_x, _y, x, y);
    }
}

int main(void)
{
    menusCreate();
    CV::init(&screenWidth, &screenHeight, "Leandro Galbarino");
    CV::run();
}
