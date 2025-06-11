#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Bezier.h"
#include "Objetos3D.h"

#define SCREEN_WIDTH 1980
#define SCREEN_HEIGHT 1080
#define TEXT_POSITON_X SCREEN_HEIGHT - 200
#define TEXT_POSITON_Y -100

int screenWidth;
int screenHeight;
int pointIndex;
bool mousePressed;
Bezier curve;
Objetos3D object;
Vector2 origem(200, 200);
Vector2 divisor(Vector2(SCREEN_WIDTH / 2 - 600, SCREEN_HEIGHT / 2 - 400));
Vector2 center(Vector2(SCREEN_WIDTH / 2 - origem.x, SCREEN_HEIGHT / 2 - origem.y));

void drawControls()
{
   Vector2 position = Vector2(TEXT_POSITON_X, TEXT_POSITON_Y);
   CV::text(position.x, position.y + 20, "CONTROLS");
   CV::text(position.x, position.y, "A e D | Rotacao no Eixo X.");
   CV::text(position.x, position.y - 20, "W e S | Rotacao no Eixo Y.");
   CV::text(position.x, position.y - 40, "Q e E | Rotacao no Eixo Z.");
   CV::text(position.x, position.y - 60, "+ e - | Alterar numero de faces.");
   CV::text(position.x, position.y - 80, "P | Alterar tipo de perspectiva.");
}

void render()
{
   CV::translate(origem.x, origem.y);
   CV::line(Vector2(divisor.x, -origem.y), Vector2(divisor.x, SCREEN_HEIGHT + origem.y));
   object.render();
   curve.render();
   drawControls();
}
// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   switch (key)
   {
   case 'A':
   case 'a':
      object.angleY -= 0.1f;
      break;
   case 'D':
   case 'd':
      object.angleY += 0.1f;
      break;
   case 'W':
   case 'w':
      object.angleX += 0.1f;
      break;
   case 'S':
   case 's':
      object.angleX -= 0.1f;
      break;
   case 'Q':
   case 'q':
      object.angleZ -= 0.1f;
      break;
   case 'E':
   case 'e':
      object.angleZ += 0.1f;
      break;
   case 'L':
   case 'l':
      object.posZ += 10;
      break;
   case 'P':
   case 'p':
      object.perspectiva = !object.perspectiva;
      break;
   case '+':
      if (object.M < 99 || object.N < 99)
      {
         object.M++;
         object.N++;
         object.fillMesh();
      }
      break;
   case '-':
      if (object.M > 3)
         object.M--;
      if (object.N > 3)
         object.N--;
      object.fillMesh();
      break;
   }
   object.fillMesh(); 
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{

   if (state == 0 && !mousePressed)
   {
      Vector2 p = Vector2(x, y);
      int i = curve.collide(p, origem);
      if (i != -1)
      {
         mousePressed = true;
         pointIndex = i;
      }
   }
   else if (state == 1)
   {
      mousePressed = false;
   }
   else if (state == -2 && mousePressed)
   {
      float offsetY = 170;
      float offsetX = 500;
      float minX = 0;
      float maxX = center.x - offsetX;
      float minY = 0;
      float maxY = SCREEN_HEIGHT - origem.y - offsetY;

      Vector2 novoPonto = Vector2(x, y) - origem;

      if (novoPonto.x >= minX && novoPonto.x <= maxX &&
          novoPonto.y >= minY && novoPonto.y <= maxY)
      {
         curve.p[pointIndex] = novoPonto;
      }
      else
      {
         // Corrigir para dentro dos limites
         if (novoPonto.x < minX)
            novoPonto.x = minX + 5;
         if (novoPonto.x > maxX)
            novoPonto.x = maxX - 5;
         if (novoPonto.y < minY)
            novoPonto.y = minY + 5;
         if (novoPonto.y > maxY)
            novoPonto.y = maxY - 5;

         curve.p[pointIndex] = novoPonto;
      }

      // Atualizar o objeto com nova curva
      object.set(curve, divisor);
      object.fillMesh();
   }
}

void createBezier()
{
   Vector2 p0 = Vector2(0, 0);
   Vector2 p1 = Vector2(200, 50);
   Vector2 p2 = Vector2(50, 40);
   Vector2 p3 = Vector2(100, 380);
   curve.set(p0, p1, p2, p3);
}

void createObject()
{
   object.set(curve, divisor);
   object.fillMesh();
}

void defaultVariables()
{
   screenWidth = 1980;
   screenHeight = 1020;
   pointIndex = -1;
   mousePressed = false;
}

int main(void)
{
   createBezier();
   createObject();
   defaultVariables();

   CV::init(&screenWidth, &screenHeight, "Leandro Galbarino");
   CV::run();
}
