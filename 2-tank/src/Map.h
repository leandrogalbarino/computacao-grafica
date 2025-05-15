#ifndef __MAP_H__
#define __MAP_H__
#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Tank.h"
#include "Barrels.h"

#define X1 -500
#define X2 500
#define Y1 -300
#define Y2 300

#define DANO_TANK 20
#define ENEMIES 30
class Map
{
public:
  Vector2 p1, p2;
  Tank *tank;
  Barrels *barrels;
  Map(Tank *tank, Barrels *barrels) : tank(tank), barrels(barrels)
  {
    p1 = Vector2(X1, Y1) + tank->origem;
    p2 = Vector2(X2, Y2) + tank->origem;
    barrels->createRandom(p1, p2, ENEMIES);
  }

  void render()
  {
    CV::rect(p1, p2);
    barrels->drawBarrels();
    tank->render();
  }

  void collideProj(Vector2 projectile)
  {
    if (projectile.x <= p1.x || projectile.x >= p2.x || projectile.y <= p1.y || projectile.y >= p2.y)
    {
      tank->setProjectil(false);
    }
  }
  void collideTank()
  {
    for (int i = 0; i < 4; i++)
    {
      float tankX = tank->tankRect[i].x + tank->origem.x;
      float tankY = tank->tankRect[i].y + tank->origem.y;
      if (tankX <= p1.x || tankX >= p2.x || tankY <= p1.y || tankY >= p2.y)
      {
        tank->life -= DANO_TANK;
        tank->dir = tank->dir * -1;
        break;
      }
    }
  }
};
#endif