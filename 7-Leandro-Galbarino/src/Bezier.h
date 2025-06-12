#ifndef __BEZIER_H__
#define __BEZIER_H__
// Classe que manipula a curva de bezier, tem seus pontos e sua equação, renderiza a curva e verifica colisão nos seus pontos para serem atualizados, renderiza a curva e os eixos X e Y para melhor visualização.

#include "Vector2.h"
#include "Vector3.h"
#define EIXO_Y 402
#define EIXO_X 302
#define PONTOS_CONTROLE 4
class Bezier
{
public:
  Vector2 p[4];
  int indexPoint; // Ponto clicado, -1 se não estiver ativo.
  // Init da curva.
  void set(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)
  {
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    indexPoint = -1;
  }

  // Curva de Bezier
  Vector2 evaluate(float t) const
  {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return p[0] * uuu + p[1] * (3 * uu * t) + p[2] * (3 * u * tt) + p[3] * ttt;
  }

  // Verifica se a curva está dentro dos limites e altera os pontos.
  int limits(Vector2 point, Vector2 origem, Vector2 center, int screenHeight)
  {
    float offsetX = 500;
    float offsetY = 240;
    float minX = 0;
    float maxX = center.x - offsetX;
    float minY = 0;
    float maxY = screenHeight - origem.y - offsetY;

    Vector2 novoPonto = point - origem;

    if (novoPonto.x < minX)
      novoPonto.x = minX + 5;
    else if (novoPonto.x > maxX)
      novoPonto.x = maxX - 5;
    if (novoPonto.y < minY)
      novoPonto.y = minY + 5;
    else if (novoPonto.y > maxY)
      novoPonto.y = maxY - 5;

    p[indexPoint] = novoPonto;
  }

  // Para detectar que um dos pontos foi clicado, usado para mudar os pontos curva de bezier
  bool collide(Vector2 _p, Vector2 origem)
  {
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      int tx = _p.x - origem.x;
      int ty = _p.y - origem.y;

      float dx = p[i].x - tx;
      float dy = p[i].y - ty;
      if (dx * dx + dy * dy < 400) // 10_000
      {
        indexPoint = i;
        return true;
      }
    }
    indexPoint = -1;
    return false;
  }

  void drawEixoXY()
  {
    CV::color(RED);
    CV::line(Vector2(0, 0), Vector2(0, 400));
    CV::line(Vector2(0, 0), Vector2(300, 0));
    CV::color(BLACK);
    CV::text(0, EIXO_Y, "Y");
    CV::text(EIXO_X, 0, "X");
  }
  // Renderiza a curva e os eixo x e y, para melhor visualização
  void render()
  {
    // Pontos de controle
    CV::color(BLACK);
    for (int i = 0; i < PONTOS_CONTROLE; i++)
    {
      CV::circleFill(p[i], 5, 20);
    }
    drawEixoXY();

    CV::color(RED);
    Vector2 last = p[0];
    for (float t = 0; t <= 1; t += 0.01)
    {
      Vector2 point = evaluate(t);
      CV::line(last, point);
      last = point;
    }
  }
};
#endif