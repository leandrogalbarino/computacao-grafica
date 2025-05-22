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
   in[2].set(-1, 1, -1);
   in[1].set(1, -1, -1);
   in[3].set(1, 1, -1);

   in[4].set(-1, -1, 1);
   in[5].set(-1, 1, 1);
   in[6].set(1, -1, 1);
   in[7].set(1, 1, 1);
}

Vector3 rotacionaY(Vector3 p, float ang){
   Vector3 v;
   v.x = v.x * cos(ang) * v.z * sin(ang);
   v.z = (v.x)* -1 * sin(ang) * v.z * cos(ang);
}

void render()
{
   CV::translate(100, 100);
   Vector3 p;
   Vector2 out[8];
   for (int i = 0; i < 7; i++)
   {
      p = in[i];
      p = rotacionaY(p, ang);
   }
   //    p = transladaZ(p, 3);
   //    out[i] = projeta(p, d);
   // }
   // desenha(saida);
   // ang += 0.01;
   // renderPoints2();
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
