/*
 * BARRELS.H
 *
 * Gerencia a criação, colisão, vida e renderização de barris no jogo.
 * - Os barris só mostram vida quando danificados para evitar poluição visual
 * - Sistema otimizado de detecção de colisão com projéteis
 * - Controle individualizado de vida para cada barril
 *
 * Design:
 * - Barris são representados como círculos vermelhos
 * - Barra de vida verde/vermelha aparece acima dos barris danificados
 * - Cada hit reduz 50 de vida (barris começam com 100)
 */

#ifndef __BARRELS_H__
#define __BARRELS_H__

#include <vector>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

#define SHOOT_RADIUS 7
#define WIDTH_LIFE 20
#define BARREL_DANO_TANK 50

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
  ~Barrels()
  {
    for (Barrel *b : barrels)
    {
      delete b;
    }
  }

  std::vector<Barrel *> getBarrels()
  {
    return barrels;
  }

  void createBarrel(Vector2 p)
  {
    Barrel *barrel = new Barrel();
    barrel->life = 100;
    barrel->maxLife = 100;
    barrel->point = p;
    barrels.push_back(barrel);
  }

  // Desenha barra de vida (só aparece quando danificado)
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

  // Aplica dano ao barril (retorna true se destruído)
  bool updateBarrelLife(Barrel *barrelToRemove)
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
          return true;
        }
      }
    }
    return false;
  }

  // Verifica colisão entre o segmento
  Barrel *collideBarrel(Vector2 pOld, Vector2 pNew)
  {

    Vector2 d = pNew - pOld; // vetor de movimento
    if (d.x == 0 && d.y == 0)
    {
      for (auto it = barrels.begin(); it != barrels.end(); ++it)
      {
        float dx = pOld.x - (*it)->point.x;
        float dy = pOld.y - (*it)->point.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= SHOOT_RADIUS)
          return *it;
      }
      return nullptr;
    }

    for (auto it = barrels.begin(); it != barrels.end(); ++it)
    {
      Vector2 f = pOld - (*it)->point;
      float a = d.dot(d);
      float b = 2 * f.dot(d);
      float c_ = f.dot(f) - SHOOT_RADIUS * SHOOT_RADIUS;
      float discriminant = b * b - 4 * a * c_;

      if (discriminant >= 0)
      {
        discriminant = sqrt(discriminant);
        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);
        if ((t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1))
        {
          return (*it);
        }
      }
    }
    return nullptr;
  }
  
  // Renderiza todos os barris
  void drawBarrels()
  {
    CV::color(RED);
    for (auto barrel : barrels)
    {
      CV::circleFill(barrel->point, 5, 20);
      drawLife(barrel);
    }
  }
};

#endif