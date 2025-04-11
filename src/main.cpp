/*********************************************************************
//  nome: Leandro Oliveira Galbarino do Nascimento
    Foram realizadas todas implementações obrigatórias:
    - Desenho com botão esquerdo do mouse
    - Criação de diferentes camadas
    - É possível ver qual camada está selecionada e alterar a camada.
    - É possível adicionar mais imagens no botão [+] presente na interface, podendo adicionar quantas imagens quiser(Para isso é necessário adicionar mais imagens no array img[] presente em Menu);
    - Através dos sliders é possível escolher a cor que deseja adicionar - com RGB.
    - É possível fazer o flip da imagem (verticalmente e horizontalmente).
    - Possível através do slider mudar o brilho da imagem
    - Pode deixar visivel ou invisivel uma camada através da checkBox ao lado do botão da imagem.
    - Foi usado botões, sliders e checkbox.
    Extras:
    -   A camada é composta pela imagem e pode ser adicionada formas geometricas e desenhar, porém são coisas diferente dentro da camada sendo assim flip e brilho mudam apenas as imagens e os desenho continuam intactos.
    -   É possível adicionar Retangulos, Circulos como formas geometricas
    -   É possível usar "lápis" que é uma linha que pode ser usada segurando o botão esquerdo do mouse.
    -   É possível usar uma especie de pincel que altera seu raio através do Slider(RAIO), ele funciona da mesma maneira que o lápis, ou seja, ao segurar ele cria circulos continuamente. Foi usado circulo do canvas e não ponto, pois ponto é um pixel que mal da para enxergar, enquanto o círculo pode ficar maior atráves do raio, e menor também, parendo um pixel.
    -   Através da vassoura é possível apagar formas e desenho(não se aplica a imagem), segurando com botão esquerdo irá apagar elementos adicionados na camada.
    - Foi usado diferentes arquivos e classes da linguagem C++ para realizar a implementação do PhotoShop Caseiro
    *********************************************************************/

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
#include "Coordinates.h"

#define BUTTONS_SIDE 7

// CORES PARA O FUNDO DO PROGRAMA -> Cinza escuro
#define BACKGROUND_R 0.156
#define BACKGROUND_G 0.164
#define BACKGROUND_B 0.18

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1950, screenHeight = 1080;

const Coordinates CoordMenuLayer(1200, 0, 1250, screenHeight);
Coordinates CoordMenuSide(0, 0, 50, screenHeight);

// int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

MenuFunctions *menuSide;
MenuLayer *menuLayer;

bool mousePressed = false;

void menusCreate()
{
    menuLayer = new MenuLayer(CoordMenuLayer);
    menuSide = new MenuFunctions(CoordMenuSide, BUTTONS_SIDE);
    menuSide->setMenuLayer(menuLayer);
    menuSide->init();
    menuLayer->init();
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

void menusHandleClick(int x1, int y1, int x2, int y2, int state)
{
    menuLayer->handleClick(x1, y1, x2, y2, state);
    menuSide->handleClick(x1, y1, x2, y2, state);
}

// Background cinza escuro
void backgroundColor()
{
    CV::color(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
    CV::rectFill(0, 0, screenWidth, screenHeight);
}

void menusRender()
{
    backgroundColor();
    menuSide->render();
    menuLayer->render();
    menuLayer->renderLayers();
}

void render()
{
    menusRender();
    Sleep(10); // nao eh controle de FPS. Somente um limitador de FPS.
}

// // funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

// // funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int _x, int _y)
{
    static int x = 0; // Ponto inicial do clique ou do último ponto no arrasto
    static int y = 0;

    static int x_init = 0;
    static int y_init = 0;

    if (state == 0) // botão pressionado
    {
        mousePressed = true;
        menusCollision(_x, _y); // verifica se clicou em botão/menu
        x = _x;
        y = _y;
        x_init = _x;
        y_init = _y;
    }
    else if (state == 1) // botão liberado
    {
        mousePressed = false;
        menusHandleClick(x_init, y_init, x, y, state); // executa ação com ponto inicial -> final
    }
    else if (state == -2 && mousePressed) // arrasto com botão pressionado
    {
        menusHandleClick(_x, _y, x, y, state); // pode usar para desenhar enquanto arrasta
        x = _x;
        y = _y;
    }
}

int main(void)
{
    menusCreate();
    CV::init(&screenWidth, &screenHeight, "Leandro Galbarino");
    CV::run();
}
