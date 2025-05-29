#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"
#include <ctime>

#define QUADRADO 4
#define CIRCULO 20
#define TRIANGULO 3
// #include <cmath>

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

int mouseX, mouseY; // variaveis globais do mouse para poder exibir dentro da render().

float ang_init = 2 * PI;

void desenhaCirculoPonto()
{
   CV::color(1, 0, 0);

   const double pi = std::acos(-1);
   double x = 0.0;
   double y = 0.0;
   float radius = 100;

   for (double a = 0; a <= 2 * pi; a += 0.1)
   {
      x = radius * cos(a);
      y = radius * sin(a);
      CV::point(x, y);
   }
}

void desenhaCirculoLinhas()
{
   float radius = 60;
   float passo = 2 * PI / 50;
   float x_ant = 0.0, y_ant = 0.0, x = 0.0, y = 0.0;
   CV::color(0.6, 0.8, 0);

   float ang;
   for (ang = 0; ang <= 2.01 * PI; ang += passo)
   {
      if (ang < passo / 2.0)
      {
         x_ant = radius * cos(ang);
         y_ant = radius * sin(ang);
         continue;
      }
      x = radius * cos(ang);
      y = radius * sin(ang);

      CV::line(x_ant, y_ant, x, y);
      x_ant = x;
      y_ant = y;
   }
}

void desenhaQuadrado()
{
   CV::color(1, 0, 0);

   const double pi = std::acos(-1);
   double x1, x2, y1, y2 = 0.0;
   float radius = 50;
   float passo = 2 * pi / QUADRADO;

   for (double a = PI / 4; a - PI / 4 < 2 * pi; a += passo)
   {
      x1 = radius * cos(a);
      y1 = radius * sin(a);
      x2 = radius * cos(a + passo);
      y2 = radius * sin(a + passo);
      CV::line(x1, y1, x2, y2);
   }
}

void desenhaRelogio()
{

   desenhaCirculoLinhas();

   float radius = 50;
   std::time_t now = time(0);
   std::tm *localTime = localtime(&now);

   float segundos = localTime->tm_sec;
   float anguloSegundos = 2 * PI * (segundos / 60);
   float xS = radius * cos(anguloSegundos - PI / 2);
   float yS = radius * sin(anguloSegundos + PI / 2);
   CV::line(0, 0, xS, yS);

   float minutos = localTime->tm_min;
   float anguloMin = 2 * PI * (minutos / 60);
   float xM = radius * cos(anguloMin - PI / 2);
   float yM = radius * sin(anguloMin + PI / 2);
   CV::color(0, 1, 0);
   CV::line(0, 0, xM, yM);

   float horas = localTime->tm_hour;
   float anguloHoras = 2 * PI * (horas / 12);
   float xH = radius * cos(anguloHoras - PI / 2);
   float yH = radius * sin(anguloHoras + PI / 2);
   CV::color(0, 0, 1);
   CV::line(0, 0, xH, yH);
}

void desenhaEngrenagem()
{
   float radius = 60;
   float radius2 = 50;
   float lado = 24;
   float passo = 2 * PI / lado;
   CV::color(1, 0, 0);
   int count = 0;
   float x1, y1, x2, y2 = 0;
   float xm1, ym1, xm2, ym2 = 0;

   for (float a = ang_init; a - ang_init <= 2.01 * PI; a += passo)
   {
      if (count % 2 == 0)
      {
         x1 = radius * cos(a);
         y1 = radius * sin(a);
         x2 = radius * cos(a + passo);
         y2 = radius * sin(a + passo);
         if (xm1 == 0)
         {
            xm1 = x2;
            ym1 = y2;
         }
         if (count != 0)
         {
            CV::line(xm2, ym2, x1, y1);
         }
         CV::line(x1, y1, x2, y2);
      }
      else
      {
         xm1 = radius2 * cos(a);
         ym1 = radius2 * sin(a);
         xm2 = radius2 * cos(a + passo);
         ym2 = radius2 * sin(a + passo);
         CV::line(x2, y2, xm1, ym1);
         CV::line(xm1, ym1, xm2, ym2);
      }
      count += 1;
   }

   radius = 40;
   for (float a = 0; a < 2 * PI; a += passo)
   {
      x1 = radius * cos(a);
      y1 = radius * sin(a);
      x2 = radius * cos(a + passo);
      y2 = radius * sin(a + passo);
      CV::line(x1, y1, x2, y2);
   }
}

void desenhaSerra()
{
   float radius = 60;
   float radius2 = 50;
   float lado = 30;
   float passo = 2 * PI / lado;
   CV::color(1, 0, 0);
   int count = 0;
   float x1, y1, x2, y2 = 0;
   float xm1, ym1, xm2, ym2 = 0;

   for (float a = ang_init; a - ang_init <= 2.01 * PI; a += passo)
   {
      if (count % 2 == 0)
      {
         x1 = radius * cos(a);
         y1 = radius * sin(a);
         x2 = radius * cos(a + passo);
         y2 = radius * sin(a + passo);
         if (xm1 == 0)
         {
            xm1 = x2;
            ym1 = y2;
         }
         if (count != 0)
         {
            CV::line(xm2, ym2, x1, y1);
         }
         CV::line(x1, y1, x2, y2);
      }
      else
      {
         xm2 = radius2 * cos(a + passo);
         ym2 = radius2 * sin(a + passo);
         CV::line(x2, y2, xm2, ym2);
      }
      count += 1;
   }

   radius = 15;
   for (float a = ang_init; a - ang_init < 2 * PI; a += passo)
   {
      x1 = radius * cos(a);
      y1 = radius * sin(a);
      x2 = radius * cos(a + passo);
      y2 = radius * sin(a + passo);
      CV::line(x1, y1, x2, y2);
   }
}

// Como a canvas cresce para baixo:`
float ang_i2 = 0;
void desenhaCaracol()
{
   CV::color(2);
   float x, y;
   float passo = 2 * PI / 400;
   float tam = 5;
   for (float ang = 0; ang < 5 * PI; ang += passo)
   {
      float r = tam * ang;
      x = r * cos(ang + ang_i2); // Aplica a rotação aqui
      y = r * sin(ang + ang_i2);
      CV::point(x, y);
   }

   ang_i2 -= 0.03;
   if (ang_i2 <= 0)
      ang_i2 += 2 * PI;
}

void desenhaEspiral()
{
   CV::color(0.6, 0.8, 0);

   float x, y, x_ant, y_ant;
   float passo = 2 * PI / 20;
   float fator = 3;

   for (float ang = ang_init; ang - ang_init < 4 * PI; ang += passo)
   {
      float radius = fator * ang;
      if (ang == ang_init)
      {
         x_ant = radius * cos(ang);
         y_ant = radius * sin(ang);
         continue;
      }
      x = radius * cos(ang);
      y = radius * sin(ang);
      CV::line(x_ant, y_ant, x, y);
      x_ant = x;
      y_ant = y;
   }
}

// float ang = 2 * PI;
void render2()
{
   // CV::translate(200, 200);
   // float passo= 100 / (2 * PI - ang);
   float radius = 20;
   float a;
   for (a = 0; radius > 0; a += 0.1)
   {
      float x = radius * cos(a);
      float y = radius * sin(a);

      radius -= 0.1;
   }
}

float ang_i = 0;

void render3()
{
   float r = 100;
   float a = ang_i;
   float decaimento = 0.1;
   float k = 0.001; // Quanto menor, mais suave

   CV::translate(300, 300);

   while (r > 0)
   {
      float x = r * cos(a);
      float y = r * sin(a);

      CV::color(2);
      CV::point(x, y);

      float passo = k / r; // Corrige a suavidade do giro
      a += passo;
      r -= decaimento;
   }

   ang_i = a;
}

void render()
{
   ang_init -= 0.03;
   if (ang_init < 0.0)
   {
      ang_init = 2 * PI;
   }
   CV::translate(250, 250);
   // desenhaCirculoPonto();
   // desenhaCirculoLinhas();
   // desenhaQuadrado();
   // desenhaEspiral();
   desenhaCaracol();
   // desenhaRelogio();
   // desenhaEngrenagem();
   // desenhaSerra();'
   // render3();

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
