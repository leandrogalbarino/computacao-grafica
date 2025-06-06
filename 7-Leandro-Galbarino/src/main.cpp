#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"

// #include <cmath>
// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
int pointIndex = -1;
bool mousePressed;
float _x;
float _y;
struct BezierCurve
{
   Vector2 p[4];

   Vector2 evaluate(float t) const
   {
      float u = 1 - t;
      float tt = t * t;
      float uu = u * u;
      float uuu = uu * u;
      float ttt = tt * t;
      return p[0] * uuu + p[1] * (3 * uu * t) + p[2] * (3 * u * tt) + p[3] * ttt;
   }
};

BezierCurve v;
void scale()
{
   for (int i = 0; i < 4; i++)
   {
      v.p[i] = v.p[i] * 50;
   }
}
void render()
{
   CV::translate(200, 200);
   for (int i = 0; i < 4; i++)
   {
      CV::circleFill(v.p[i], 10, 20);
   }

   CV::color(1);
   for (int i = 0; i < 4; i++)
   {
      Vector2 last = v.p[0];
      for (float t = 0; t <= 1; t += 0.01)
      {
         Vector2 point = v.evaluate(t);
         CV::line(last, point);
         last = point;
      }
   }
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

   if (state == 0)
   {
      if (mousePressed)
      {
         return;
      }
      else
      {
         for (int i = 0; i < 4; i++)
         {
            int tx = x - 200;
            int ty = y - 200;

            float dx = v.p[i].x - tx;
            float dy = v.p[i].y - ty;
            if (dx * dx + dy * dy < 400) // 10_000
            {
               _x = x;
               _y = y;
               mousePressed = true;
               pointIndex = i;
               return;
            }
         }
      }
   }
   else if (state == 1)
   {
      mousePressed = false;
   }
   else if (state == -2 && mousePressed)
   {
      v.p[pointIndex].x = x - 200;
      v.p[pointIndex].y = y - 200;
   }
}

int main(void)
{
   v.p[0] = Vector2(0, 1);
   v.p[1] = Vector2(3, 5);
   v.p[2] = Vector2(5, 4);
   v.p[3] = Vector2(10, 4);
   scale();
   _x = 0;
   _y = 0;
   mousePressed = false;
   CV::init(&screenWidth, &screenHeight, "Leandro Galbarino");
   CV::run();
}
