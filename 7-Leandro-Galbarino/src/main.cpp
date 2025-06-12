// Programa de entrada que manipula os eventos do mouse, teclado, e altera a curva e o objeto 3D.
// Canvas cresce para cima
// Foram implementadas todas funcionalidades básica.
// O programa tem as instruções de como fazer as manipulações.

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
#define TEXT_POSITON_Y -80

int screenWidth;
int screenHeight;
bool mousePressed;                                                                  // Controle para eventos do mouse
Bezier curve;                                                                       // Curva de Bezier
Objetos3D object;                                                                   // Objeto 3D
Vector2 origem(200, 200);                                                           // Origem translada.
Vector2 divisor(Vector2(SCREEN_WIDTH / 2 - 600, SCREEN_HEIGHT / 2 - 400));          // Posicação da divisoria
Vector2 center(Vector2(SCREEN_WIDTH / 2 - origem.x, SCREEN_HEIGHT / 2 - origem.y)); // Centro da tela

// Desenha a descrição dos controles
void drawControls()
{
   Vector2 position = Vector2(TEXT_POSITON_X, TEXT_POSITON_Y);
   CV::color(BLACK);
   CV::rectFill(Vector2(TEXT_POSITON_X - 10, TEXT_POSITON_Y + 15), Vector2(TEXT_POSITON_X + 350, TEXT_POSITON_Y - 110));

   CV::color(WHITE);
   CV::text(position.x, position.y, "CONTROLES");
   CV::text(position.x, position.y - 20, "A e D | Rotacao no Eixo X.");
   CV::text(position.x, position.y - 40, "W e S | Rotacao no Eixo Y.");
   CV::text(position.x, position.y - 60, "Q e E | Rotacao no Eixo Z.");
   CV::text(position.x, position.y - 80, "+ e - | Alterar numero de faces.");
   CV::text(position.x, position.y - 100, "P | Alterar tipo de perspectiva.");
}

// Desenha a linha divisoria entre o Objeto 3D e a curva de Bezier.
void drawDivisorLine()
{
   CV::line(Vector2(divisor.x, -origem.y), Vector2(divisor.x, SCREEN_HEIGHT + origem.y));
}

// Renderiza tudo
void render()
{
   CV::translate(origem.x, origem.y);
   object.render();
   curve.render();
   drawDivisorLine();
   drawControls();
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   object.eventsKey(key);
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
      if (curve.collide(p, origem))
         mousePressed = true;
   }
   else if (state == 1)
   {
      curve.indexPoint = -1;
      mousePressed = false;
   }
   else if (state == -2 && mousePressed)
   {
      Vector2 p = Vector2(x, y);
      curve.limits(p, origem, center, screenHeight);
      object.set(curve, divisor);
      object.fillMesh();
   }
}
// init da curva
void createBezier()
{
   Vector2 p0 = Vector2(0, 0);
   Vector2 p1 = Vector2(200, 50);
   Vector2 p2 = Vector2(50, 40);
   Vector2 p3 = Vector2(100, 380);
   curve.set(p0, p1, p2, p3);
}

// init do objeto
void createObject()
{
   object.set(curve, divisor);
   object.fillMesh();
}

void defaultVariables()
{
   screenWidth = SCREEN_WIDTH;
   screenHeight = SCREEN_WIDTH;
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
