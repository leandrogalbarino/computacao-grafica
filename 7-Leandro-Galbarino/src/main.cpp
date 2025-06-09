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

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
// #include <cmath>
// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth;
int screenHeight;
int pointIndex;
bool mousePressed;

Bezier curve;
Objetos3D object;
Vector2 origem(200, 200);

void render()
{
   CV::translate(origem.x, origem.y);
   // CV::line(Vector2(0 - origem.x, SCREEN_HEIGHT/2 - origem.y),Vector2(SCREEN_WIDTH + origem.x, SCREEN_HEIGHT/2 - origem.y));
   CV::line(Vector2(SCREEN_WIDTH/2 - origem.x,  - origem.y),Vector2(SCREEN_WIDTH/2 - origem.x, SCREEN_HEIGHT + origem.y));
   object.render();
   curve.render();
}
// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   switch (key)
   {
   case 'a':
      object.angleY -= 0.1f;
      break; // Rotação Y
   case 'd':
      object.angleY += 0.1f;
      break;
   case 'w':
      object.angleX += 0.1f;
      break; // Rotação X
   case 's':
      object.angleX -= 0.1f;
      break;
   case 'r':
      object.angleZ -= 0.1f;
      break;
   case 't':
      object.angleZ += 0.1f;
      break;
   case 'q':
      object.posZ += 10;
      break; // Translação Z
   case 'e':
      object.posZ -= 10;
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
   case 'p':
      object.perspectiva = !object.perspectiva;
      break;
   }
   object.fillMesh(); // Recalcula a malha com novas transformações
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
      curve.p[pointIndex] = Vector2(x, y) - origem;
      object.set(curve);
      object.fillMesh();
   }
}

void createBezier()
{
   Vector2 p0 = Vector2(0, 1);
   Vector2 p1 = Vector2(3, 5);
   Vector2 p2 = Vector2(5, 4);
   Vector2 p3 = Vector2(10, 4);
   curve.set(p0, p1, p0, p3);
   curve.scale();
}

void createObject()
{
   object.set(curve);
   object.fillMesh();
}

void defaultVariables()
{
   screenWidth = SCREEN_WIDTH;
   screenHeight = SCREEN_HEIGHT;
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
