#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"

// #include <cmath>
Vector2 p1, p2, p3;
Vector2 v1, v2;
Vector2 origem;

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; // variaveis globais do mouse para poder exibir dentro da render().

// 1. Complementar operadores da classe Vecto2
void desenharLinha()
{
   CV::color(1, 0, 0);
   CV::line(p1, p2);
   v1 = p2 - p1;
   v1.normalize();
   v1 = v1 * 200;
   v1 = v1 / 10;

   CV::line(origem, v1);
}

float produtoInterno(Vector2 v1, Vector2 v2)
{
   return (v1.x * v2.x + v1.y * v2.y);
}

float produto(Vector2 v1, Vector2 v2)
{
   float mod1 = v1.modulo();
   float mod2 = v2.modulo();
   return mod1 * mod2;
}

void desenhaVector(Vector2 v, int tam)
{
   CV::color(1, 0, 0);
   Vector2 esc = v * tam;
   CV::line(origem, esc);
}

void calculaAng(Vector2 v1, Vector2 v2)
{
   char text[50];
   float angRad = acos(produtoInterno(v1, v2) / produto(v1, v2));
   float angDeg = angRad * (180.0 / PI); // converter para graus

   sprintf(text, "Angulo: %.2f", angDeg);
   CV::text(200, 200, text);
}

void render()
{
   CV::translate(100, 100);
   int tamDesenho = 200;
   v1 = p2 - p1;
   v2 = p3 - p1;
   v1.normalize();
   v2.normalize();
   // calculaAng(v1, v2);

   desenhaVector(v1, tamDesenho);
   desenhaVector(v2, tamDesenho);
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
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   p2.set(x, y);
}

int main(void)
{
   origem.set(0, 0);
   p1.set(100, 100);
   p2.set(100, 100);
   p3.set(100, 500);
   CV::init(&screenWidth, &screenHeight, "Coordenadas Polares");
   CV::run();
}
