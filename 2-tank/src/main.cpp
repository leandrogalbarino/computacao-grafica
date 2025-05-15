#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Tank.h"
#include "Barrels.h"
#include "Map.h"
#include <cmath>

Tank *tank;
Barrels *barrels;
Map *map;
// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1980, screenHeight = 1080;

void render()
{
   map->render();
   if (tank->shoot)
   {
      map->collideProj(tank->shootVector);
      Barrel *barrel = barrels->collideBarrel(tank->shootVector);
      if (barrel){
         barrels->updateBarrelLife(barrel);
         tank->setProjectil(false);
      }
   }
   map->collideTank();
   Sleep(10);
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   if (key == 'a' || key == 'A')
      tank->setTurningLeft(true);
   if (key == 'd' || key == 'D')
      tank->setTurningRight(true);
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   if (key == 'a' || key == 'A')
   {
      tank->setTurningLeft(false);
   }
   if (key == 'd' || key == 'D')
   {
      tank->setTurningRight(false);
   }
   if (key == ' ')
   {
      tank->setProjectil(true);
   }
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   tank->setMousePositon(x, y);
}

void createGame()
{
   tank = new Tank();
   barrels = new Barrels();
   map = new Map(tank, barrels);
}

int main(void)
{
   createGame();
   CV::init(&screenWidth, &screenHeight, "Coordenadas Polares");
   CV::run();
}
