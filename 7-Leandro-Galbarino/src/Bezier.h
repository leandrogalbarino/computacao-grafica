#ifndef __BEZIER_H__
#define __BEZIER_H__
#include "Vector2.h"
#include "Vector3.h"

#define PONTOS_CONTROLE 4
class Bezier
{
public:
  Vector2 p[4];
  Vector2 pDraw[4];
  void set(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)
  {
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
  }

  Vector2 evaluate(float t) const
  {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return p[0] * uuu + p[1] * (3 * uu * t) + p[2] * (3 * u * tt) + p[3] * ttt;
  }

  Vector2 *scale()
  {
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      pDraw[i] = p[i] * 200;
    }
  }

  int collide(Vector2 _p, Vector2 origem)
  {
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      int tx = _p.x - origem.x;
      int ty = _p.y - origem.y;

      float dx = p[i].x - tx;
      float dy = p[i].y - ty;
      if (dx * dx + dy * dy < 400) // 10_000
      {
        return i;
      }
    }
    return -1;
  }

  void normalizeAll()
  {
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      p[i].normalize();
    }
  }

  void render()
  {
    // Pontos de controle
    CV::color(BLACK);
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      CV::circleFill(p[i], 5, 20);
    }

    // Curva de Bezier
    CV::color(RED);
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      Vector2 last = pDraw[0];
      for (float t = 0; t <= 1; t += 0.01)
      {
        Vector2 point = evaluate(t);
        CV::line(last, point);
        last = point;
      }
    }
  }
};
#endif