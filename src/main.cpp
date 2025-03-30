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

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1200, screenHeight = 720;

int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Bmp *img1;
unsigned char *data;
int N = 10;
int menuWidth;
Menu *menu;
//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.

void render()
{
    img1->render();
    menu->render();
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
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    static int _x = 0;  // Variável estática mantém valor entre chamadas da função
    static int _y = 0;
    if(state == 0){
        menu->colisaoBotoes(x,y);
        _x = x;
        _y = y;
    }
    if(state == 1){
        menu->setCoordenadas(_x,_y, x, y);
        menu->armazenar();
   }
}

int main(void)
{
    menu = new Menu(50,720);
    img1 = new Bmp(".\\1_Trab1\\images\\img3.bmp");
   //img1 = new Bmp(".\\1_Trab1\\images\\img1.bmp");
   //img1 = new Bmp(".\\1_Trab1\\images\\img2.bmp");

   img1->convertBGRtoRGB();
   data = img1->getImage();



   CV::init(&screenWidth, &screenHeight, "Leandro Galbarino");
   CV::run();
}
