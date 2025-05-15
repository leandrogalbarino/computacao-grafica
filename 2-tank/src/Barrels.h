#ifndef __BARRELS_H__
#define __BARRELS_H__

#include <vector>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#define SHOOT_RADIUS 7
#define WIDTH_LIFE 20
typedef struct Barrel
{
  int maxLife;
  int life;
  Vector2 point;
} Barrel;

class Barrels
{
private:
  std::vector<Barrel *> barrels;

public:
  void createBarrel(Vector2 p)
  {
    Barrel *barrel = new Barrel();
    barrel->life = 100;
    barrel->maxLife = 100;
    barrel->point = p;
    barrels.push_back(barrel);
  }

  void drawLife(Barrel *barrel)
  {
    if (barrel->life <= 0 || barrel->life == barrel->maxLife)
    {
      return;
    }
    float barWidth = WIDTH_LIFE / 1.0f / barrel->maxLife;
    float startX = barrel->point.x - 10;
    float endX = barrel->point.x + 10;

    for (int i = 0; i < barrel->maxLife; i++)
    {
      // 3 - verde, 2 vermelho
      CV::color(i <= barrel->life ? GREEN : RED);
      float x1 = startX + i * barWidth;
      float x2 = x1 + 1;
      float y1 = barrel->point.y - RADIUS - 6;
      float y2 = barrel->point.y - RADIUS - 1;
      CV::rectFill(x1, y1, x2, y2);
    }
  }

  void createRandom(Vector2 p1, Vector2 p2, int enemies)
  {
    srand(time(0));
    int minX = (int)(p1.x * 100);
    int maxX = (int)(p2.x * 100);
    int minY = (int)(p1.y * 100);
    int maxY = (int)(p2.y * 100);

    for (int i = 0; i < enemies; i++)
    {
      float randomX = (minX + rand() % (maxX - minX + 1)) / 100.0f;
      float randomY = (minY + rand() % (maxY - minY + 1)) / 100.0f;
      if (!collideBarrel(Vector2(randomX, randomY)))
      {
        createBarrel(Vector2(randomX, randomY));
      }
    }
  }

  void updateBarrelLife(Barrel *barrelToRemove)
  {
    barrelToRemove->life -= 50;

    if (barrelToRemove->life <= 0)
    {
      for (auto it = barrels.begin(); it != barrels.end(); ++it)
      {
        if (*it == barrelToRemove)
        {
          delete *it;
          barrels.erase(it);
          break;
        }
      }
    }
  }

  Barrel *collideBarrel(Vector2 p)
  {
    for (auto it = barrels.begin(); it != barrels.end();)
    {
      {
        float dx = p.x - (*it)->point.x;
        float dy = p.y - (*it)->point.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= SHOOT_RADIUS)
        {
          return (*it);
        }
        else
        {
          ++it;
        }
      }
    }
    return nullptr;
  }

  void drawBarrels()
  {
    for (auto barrel : barrels)
    {
      CV::circleFill(barrel->point, 5, 20);
      drawLife(barrel);
    }
  }
};

#endif