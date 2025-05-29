#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include <ctime>

#define QUADRADO 4
#define CIRCULO 20
#define TRIANGULO 3
// #include <cmath>

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; // variaveis globais do mouse para poder exibir dentro da render().

float ang_init = 0;

float ang_i = -(PI / 2);
void desenharPonteiro()
{
   int radius = 50;
   float passo = 2 * PI / 60;
   ang_i += passo;
   float x = radius * cos(-ang_i);
   float y = radius * sin(-ang_i);
   CV::line(0, 0, x, y);
}

void desenhaQuadrado()
{
   float passo = 2 * PI / QUADRADO;
   int radius = 50;
   float ang_init = (PI / 4); // Corrigindo o ângulo
   for (float ang = ang_init; ang < (2 * PI) + ang_init; ang += passo)
   {
      float x1 = radius * cos(ang);
      float y1 = radius * sin(ang);
      float x2 = radius * cos(ang + passo);
      float y2 = radius * sin(ang + passo);
      CV::line(x1, y1, x2, y2);
   }
}

void desenharCirculoPoint()
{
   float passo = 0.00001;
   int radius = 50;
   for (float ang = 0; ang < 2 * PI; ang += passo)
   {
      float x1 = radius * cos(ang);
      float y1 = radius * sin(ang);
      CV::point(x1, y1);
   }
}

void desenharCirculoLine()
{
   float passo = 2 * PI / CIRCULO;
   int radius = 50;
   for (float ang = 0; ang < 2 * PI; ang += 0.00001)
   {
      float x1 = radius * cos(ang);
      float y1 = radius * sin(ang);
      float x2 = radius * cos(ang + passo);
      float y2 = radius * sin(ang + passo);
      CV::point(x1, y1);
   }
}

void espiral()
{
   float passo = 0.001;
   float radius = 0;
   float fator = 5;
   for (float ang = 0; ang < 10 * PI; ang += passo)
   {
      float r = radius * fator;
      float x1 = r * cos(ang + ang_init);
      float y1 = r * sin(ang + ang_init);
      float x2 = r * cos(ang + passo + ang_init);
      float y2 = r * sin(ang + passo + ang_init);
      radius += passo;
      CV::point(x1, y1);
   }
}

void serraCircular()
{
   float passo = 2 * PI / 50;
   float r1 = 50;
   float r2 = 70;
   int i = 0;
   float x1, x2, y1, y2;
   float x_1, y_1, x_2, y_2;
   for (float ang = 0; ang <= 2.0001 * PI; ang += passo)
   {
      if (i % 2 == 0)
      {
         x1 = r1 * cos(ang + ang_init);
         y1 = r1 * sin(ang + ang_init);
      }
      else
      {
         x2 = r2 * cos(ang + ang_init);
         y2 = r2 * sin(ang + ang_init);
      }
      if (i != 0)
      {
         CV::line(x1, y1, x2, y2);
      }
      i++;
   }
}

void engrenagemCircular()
{
   float passo = 2 * PI / 20;
   float r1 = 50;
   float r2 = 60;
   int i = 0;
   float x1, y1, x2, y2;
   float x_1, y_1, x_2, y_2;
   for (float ang = 0; ang <= 2.0001 * PI; ang += passo)
   {
      if (i % 2 == 0)
      {
         x1 = r1 * cos(ang + ang_init);
         y1 = r1 * sin(ang + ang_init);
         x2 = r1 * cos(ang + ang_init + passo);
         y2 = r1 * sin(ang + ang_init + passo);
      }
      else
      {
         x1 = r2 * cos(ang + ang_init);
         y1 = r2 * sin(ang + ang_init);
         x2 = r2 * cos(ang + ang_init + passo);
         y2 = r2 * sin(ang + ang_init + passo);
      }
      CV::line(x1, y1, x2, y2);
      if (i != 0)
      {
         CV::line(x_2, y_2, x1, y1);
      }
      x_1 = x1;
      y_1 = y1;
      x_2 = x2;
      y_2 = y2;
      i++;
   }
}

void render()
{
   ang_init += 0.01;
   CV::translate(200, 200);
   CV::color(2);

   // desenharPonteiro();
   // desenhaQuadrado();
   // desenharCirculoLine();
   // desenharCirculoPoint();
   espiral();
   // serraCircular();
   // engrenagemCircular();
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
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Coordenadas Polares");
   CV::run();
}
