#ifndef __MAP_H__
#define __MAP_H__

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Tank.h"
#include "Barrels.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <random>
#include <cstdlib>

#define MAP_DANO_TANK 20
#define ENEMIES 20

struct BezierCurve
{
  Vector2 p0, p1, p2, p3;

  Vector2 evaluate(float t) const
  {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return p0 * uuu + p1 * (3 * uu * t) + p2 * (3 * u * tt) + p3 * ttt;
  }
};

class Map
{
private:
  void createCurvas()
  {
    const int N = 6;
    const float radius = 300.0f;
    const float noise = 80.0f; // Controle de variação dos pontos

    Vector2 points[N];

    for (int i = 0; i < N; ++i)
    {
      float angle = 2 * M_PI * i / N;
      float r = radius + (rand() % (int)(2 * noise)) - noise; // Variação entre [-noise, +noise]
      float x = r * cos(angle);
      float y = r * sin(angle);
      points[i] = Vector2(x, y);
    }

    // Centro geométrico
    Vector2 centro(0, 0);
    for (const auto &p : points)
      centro = centro + p;
    centro = centro * (1.0f / N);

    // Curvas externas suaves (Catmull-Rom -> Bézier cúbica)
    curvas.clear();
    for (int i = 0; i < N; i++)
    {
      Vector2 p0 = points[(i - 1 + N) % N];
      Vector2 p1 = points[i];
      Vector2 p2 = points[(i + 1) % N];
      Vector2 p3 = points[(i + 2) % N];

      Vector2 b0 = p1 - centro + tank->origem;
      Vector2 b1 = p1 + (p2 - p0) / 6.0f - centro + tank->origem;
      Vector2 b2 = p2 - (p3 - p1) / 6.0f - centro + tank->origem;
      Vector2 b3 = p2 - centro + tank->origem;

      curvas.push_back({b0, b1, b2, b3});
    }

    // Curvas internas
    float minDistance = 500.0f;
    scale = 0.5f;
    bool scaleValid = false;
    while (!scaleValid && scale > 0.1f)
    {
      scaleValid = true;
      for (int i = 0; i < N; i++)
      {
        Vector2 pInternal = centro + (points[i] - centro) * scale;
        Vector2 pExternal = points[i];
        if ((pInternal - pExternal).length() > minDistance)
        {
          scale -= 0.05f;
          scaleValid = false;
          break;
        }
      }
    }

    // Gera pontos internos
    Vector2 ip[6];
    for (int i = 0; i < N; i++)
      ip[i] = centro + (points[i] - centro) * scale;

    for (int i = 0; i < N; i++)
    {
      Vector2 p0 = ip[(i - 1 + N) % N];
      Vector2 p1 = ip[i];
      Vector2 p2 = ip[(i + 1) % N];
      Vector2 p3 = ip[(i + 2) % N];

      Vector2 b0 = p1 - centro + tank->origem;
      Vector2 b1 = p1 + (p2 - p0) / 6.0f - centro + tank->origem;
      Vector2 b2 = p2 - (p3 - p1) / 6.0f - centro + tank->origem;
      Vector2 b3 = p2 - centro + tank->origem;

      curvas.push_back({b0, b1, b2, b3});
    }

    float safeScale = (1.0f + scale) * 0.5f;
    Vector2 safePosition = centro + (points[0] - centro) * safeScale + (tank->origem - centro);
    tank->origem = safePosition;
  }

  void createEnemies()
  {
    int created = 0;
    int attempts = 0;

    float minX = -500.0f;
    float maxX = 500.0f;
    float minY = -500.0f;
    float maxY = 500.0f;

    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    while (created < ENEMIES && attempts < ENEMIES * 50)
    {
      float x = distX(*gen);
      float y = distY(*gen);
      Vector2 ponto(x + tank->origem.x, y + tank->origem.y);

      if (isPointInsideClosedArea(ponto) && !barrels->collideBarrel(ponto, ponto))
      {
        barrels->createBarrel(ponto);
        created++;
      }

      attempts++;
    }
  }

  bool isPointInsideClosedArea(Vector2 point)
  {
    int intersections = 0;

    for (const auto &curva : curvas)
    {
      Vector2 last = curva.p0;
      for (float t = 0.01f; t <= 1.0f; t += 0.01f)
      {
        Vector2 current = curva.evaluate(t);

        if ((last.y > point.y) != (current.y > point.y))
        {
          float xIntersect = (point.y - last.y) * (current.x - last.x) / (current.y - last.y) + last.x;
          if (point.x < xIntersect)
          {
            intersections++;
          }
        }
        last = current;
      }
    }

    return (intersections % 2) == 1;
  }

public:
  std::vector<BezierCurve> curvas;
  Tank *tank;
  Barrels *barrels;
  std::mt19937 *gen;
  Vector2 pScore;
  float scale;

  Map(Tank *tank, Barrels *barrels, std::mt19937 *gen) : tank(tank), barrels(barrels), gen(gen)
  {
    createCurvas();
    createEnemies();
    scale = 0.6;
    pScore = Vector2(-500 + tank->origem.x, -380 + tank->origem.y);
  }

  void drawCurves()
  {
    CV::color(BLACK);
    for (const auto &curva : curvas)
    {
      Vector2 last = curva.p0;
      for (float t = 0; t <= 1.0f; t += 0.01f)
      {
        Vector2 point = curva.evaluate(t);
        CV::line(last, point);
        last = point;
      }
    }
  }

  void drawScore()
  {
    char scoreText[50];
    sprintf(scoreText, "Pontuacao: %d", tank->score);
    CV::color(BLACK);
    CV::text(pScore, scoreText);
  }

  void render()
  {
    drawCurves();
    drawScore();
    barrels->drawBarrels();
    tank->render();
  }

  void collideProj(Vector2 projectile)
  {
    if (tank->shoot && !isPointInsideClosedArea(projectile))
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
      Vector2 pos(tankX, tankY);
      clock_t now = clock();
      double elapsedMs = (double)(now - tank->lastCollision) * 1000.0 / CLOCKS_PER_SEC;

      if (!isPointInsideClosedArea(pos) && elapsedMs > COLLISION_COOLDOWN_MS)
      {
        tank->UpdateCollision(MAP_DANO_TANK, now);
        break;
      }
    }
  }
};

#endif
