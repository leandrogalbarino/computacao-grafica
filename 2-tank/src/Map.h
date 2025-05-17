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
#define ENEMIES 3


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

  void drawScore()
  {
    char scoreText[50];
    sprintf(scoreText, "Pontuacao: %d", tank->score);
    Vector2 pText = Vector2(p1.x, p1.y - 20);
    CV::color(BLACK);
    CV::text(pText, scoreText);
  }

  void render()
  {
    CV::rect(p1, p2);
    drawScore();
    barrels->drawBarrels();
    tank->render();
  }

    // static float timeAccumulator = 0.0f;
    // static int frameCount = 0;
    // static int fps = 0;

    // timeAccumulator += deltaTime;
    // frameCount++;

    // if (timeAccumulator >= 1.0f) {
    //     fps = frameCount;
    //     frameCount = 0;
    //     timeAccumulator = 0.0f;
    // }

    // // mostra o FPS na tela
    // CV::color(0); // preto
    // std::stringstream ss;
    // ss << "FPS: " << fps;
    // CV::text(10, height - 20, (char*)ss.str().c_str());

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