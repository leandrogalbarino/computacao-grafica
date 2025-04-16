// Autor: Leandro Oliveira Galbarino do Nascimento
// obs: O eixo Y cresce para baixo.
//
// Implementações obrigatórias concluídas:
//
// - Desenho com o botão esquerdo do mouse.
// - Criação e gerenciamento de diferentes camadas.
// - É possível visualizar e alternar a camada ativa.
// - Botão [+] permite adicionar novas imagens dinamicamente.
//     (Para isso, basta adicionar novos caminhos no array `img[]` em `Menu.h`)
// - Sliders para seleção de cor (RGB).
// - Flip da imagem (horizontal e vertical).
// - Controle de brilho da imagem através de slider.
// - Controle de visibilidade de camadas com checkboxes.
// - Interface composta por botões, sliders e checkboxes.
//
// Funcionalidades Extras:
//
// - Camadas compostas por imagem + formas geométricas/desenho:
//     → Flip e brilho afetam apenas a imagem, os desenhos permanecem intactos.
// - Adição de formas geométricas: Retângulo e Círculo.
// - Ferramenta "Lápis": desenha linhas ao segurar o botão esquerdo do mouse.
// - Ferramenta "Pincel": desenha círculos contínuos, com raio ajustável via slider (RAIO).
//     → Círculo usado no lugar do ponto para melhor visualização (ponto é um pixel pequeno demais).
// - Ferramenta "Vassoura": permite apagar formas geométricas e desenhos (não afeta a imagem).
//     → Segure o botão esquerdo do mouse sobre o que deseja apagar.
// - Estrutura modular utilizando múltiplos arquivos e classes em C++.
//
// Instruções Gerais de Uso:
// - Use os botões laterais da direita para adicionar e controlar as camadas.
// - Os sliders controlam as cores para novos desenhos e o brilho das imagens.
// - Clique nos botões geométricos da esquerda para escolher a forma a ser desenhada.
// - Desenhe segurando o botão esquerdo do mouse na área de edição.
// - O botão "+" adiciona uma nova camada com imagem.
// - A camada ativa fica com borda vermelha no botão.
// - Use os botões de seta ↑ ↓ para alterar a ordem das camadas.
// - A checkbox de cada camada alterna a visibilidade da respectiva camada.
// - A última ferramenta (vassoura) permite apagar formas/desenhos de uma camada.

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "bmp.h"
#include "checkbox.h"
#include "Menu.h"
#include "Vector2.h"
#include <iostream>
#include <string>
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
