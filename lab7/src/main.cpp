#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Vector3.h"

// #include <cmath>
// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

Vector3 in[8];
Vector2 out[8];
float ang = 0;
int mouseX, mouseY; // variaveis globais do mouse para poder exibir dentro da render().

void createPoints()
{
   in[0].set(-1, -1, -1);
   in[1].set(-1, 1, -1);
   in[2].set(1, -1, -1);

   in[3].set(1, 1, -1);

   in[4].set(-1, -1, 1);
   in[5].set(-1, 1, 1);
   in[6].set(1, -1, 1);

   in[7].set(1, 1, 1);
}

void desenhaCubo(Vector2 v[])
{
   // 0 - 1, 0 - 2, 0 - 4
   // 1 - 3 , 1 - 5
   // 2 - 3 , 2 - 6
   // 3 - 7
   // 4 - 5, 4 - 6
   // 5 - 7
   // 6 - 7
   CV::color(2);
   CV::line(v[0], v[1]);
   CV::line(v[0], v[2]);
   CV::line(v[0], v[4]);

   CV::line(v[1], v[3]);
   CV::line(v[1], v[5]);

   CV::line(v[2], v[3]);
   CV::line(v[2], v[6]);

   CV::line(v[3], v[7]);

   CV::line(v[4], v[5]);
   CV::line(v[4], v[6]);

   CV::line(v[5], v[7]);
   CV::line(v[6], v[7]);
}
Vector3 rotacionaY(Vector3 p, float ang)
{
   Vector3 pAux = p;
   p.x = pAux.x * cos(ang) + pAux.z * sin(ang);
   p.z = (pAux.x) * -1 * sin(ang) + pAux.z * cos(ang);
   return p;
}

Vector3 transladaZ(Vector3 p, int translate)
{
   p.z = p.z + translate;
   return p;
}

Vector2 projeta(Vector3 p, int d)
{
   Vector2 p2;
   p2.x = p.x * d / p.z;
   p2.y = p.y * d / p.z;
   return p2;
}

Vector3 escala(Vector3 p, int escala){
   p = p * escala;
   return p;
}
void render()
{
   CV::translate(200, 200);
   Vector3 p;
   Vector2 out[8];
   int d = 200;
   int transladar = 200;
   for (int i = 0; i < 8; i++)
   {
      p = in[i];
      p = rotacionaY(p, ang);
      p = escala(p, 50);
      p = transladaZ(p, transladar);
      out[i] = projeta(p, d);
   }
   desenhaCubo(out);
   ang -= 0.01;
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
   createPoints();
   CV::init(&screenWidth, &screenHeight, "Coordenadas Polares");
   CV::run();
}
