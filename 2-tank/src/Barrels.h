#ifndef __BARRELS_H__
#define __BARRELS_H__

#include <vector>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include <cmath>
typedef struct Barrel
{
  int life;
  Vector2 point;
};

class Barrels
{
private:
  std::vector<Barrel> barrels;

public:
  void createBarrel(Vector2 p)
  {
    Barrel barrel;
    barrel.life = 100;
    barrel.point = p;
    barrels.push_back(barrel);
  }

  void collideBarrel(Vector2 shoot)
  {
    for (auto it = barrels.begin(); it != barrels.end();)
    {
      {
        float dx = shoot.x - it->point.x;
        float dy = shoot.y - it->point.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= 20)
        {
          it->life -= 50;
          if (it->life <= 0)
          {
            it = barrels.erase(it);
            break;
          }
        }
        else
        {
          ++it;
        }
      }
    }
  }

  void drawBarrels()
  {
    for (auto &barrel : barrels)
    {
      CV::circleFill(barrel.point, 20, 20);
    }
  }
};

#endif