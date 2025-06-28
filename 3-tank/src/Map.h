/*
  MAP.H
 Gerencia o mapa do jogo e suas interações:
  - Cria pistas usando curvas de Bézier
  - Posiciona barris (inimigos) aleatoriamente
  - Detecta colisões do tanque e projéteis com a pista
  - Renderiza todos os elementos do mapa
 */

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

// Constantes do jogo
#define MAP_DANO_TANK 20   // Dano ao tanque por colisão com a pista
#define ENEMIES 20         // Número máximo de barris/inimigos
#define ALTER_MAP_POINTS 1 // Modo de edição de pista

// Estrutura para curvas de Bézier (criação da pista)
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
  std::vector<BezierCurve> curvas; // Armazena as curvas da pista
  Tank *tank;                      // Referência ao tanque do jogador
  Barrels *barrels;                // Gerenciador de barris/inimigos
  std::mt19937 *gen;               // Gerador de números aleatórios
  float scale;                     // Escala para pista interna
  Vector2 pScore;                  // Posição do placar

  // Métodos auxiliares para criação e validação da pista
  bool ccw(Vector2 A, Vector2 B, Vector2 C)
  {
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
  }

  bool onSegment(Vector2 p, Vector2 q, Vector2 r)
  {
    return std::min(p.x, r.x) <= q.x && q.x <= std::max(p.x, r.x) &&
           std::min(p.y, r.y) <= q.y && q.y <= std::max(p.y, r.y);
  }

  bool segmentsIntersect(Vector2 A, Vector2 B, Vector2 C, Vector2 D)
  {
    if (ccw(A, C, D) != ccw(B, C, D) &&
        ccw(A, B, C) != ccw(A, B, D))
      return true;

    return onSegment(A, C, B) || onSegment(A, D, B) ||
           onSegment(C, A, D) || onSegment(C, B, D);
  }

  bool isAdjacent(int i, int j, int N)
  {
    return (abs(i - j) == 1) || (i == 0 && j == N - 1) || (j == 0 && i == N - 1);
  }

  // Funcões para validar a pista.
  bool isTrackValid(std::vector<Vector2> &points)
  {
    int N = points.size();
    if (N < 3)
      return false;

    for (int i = 0; i < N; ++i)
    {
      if ((points[i] - points[(i + 1) % N]).length() < 1.0f)
        return false;
    }

    for (int i = 0; i < N; ++i)
    {
      Vector2 A = points[i];
      Vector2 B = points[(i + 1) % N];

      for (int j = i + 1; j < N; ++j)
      {
        if (isAdjacent(i, j, N))
          continue;

        Vector2 C = points[j];
        Vector2 D = points[(j + 1) % N];

        if (segmentsIntersect(A, B, C, D))
          return false;
      }
    }

    return true;
  }

  // Calcula uma posicão dentro das curvas para o tank
  Vector2 calculateSafePositon(std::vector<Vector2> &pointsInput)
  {
    Vector2 center = calculateCenter(pointsInput);
    float safeScale = (1.0f + scale) * 0.5f;
    Vector2 safePosition = center + (pointsInput[0] - center) * safeScale + (tank->origem - center);
    return safePosition;
  }

  // Adiciona a curva as curvas
  void addBezierCurves(std::vector<Vector2> points)
  {
    int size = points.size();
    Vector2 centro = calculateCenter(points);

    for (int i = 0; i < size; i++)
    {
      Vector2 p0 = points[(i - 1 + size) % size];
      Vector2 p1 = points[i];
      Vector2 p2 = points[(i + 1) % size];
      Vector2 p3 = points[(i + 2) % size];

      Vector2 b0 = p1 - centro + tank->origem;
      Vector2 b1 = p1 + (p2 - p0) / 6.0f - centro + tank->origem;
      Vector2 b2 = p2 - (p3 - p1) / 6.0f - centro + tank->origem;
      Vector2 b3 = p2 - centro + tank->origem;

      curvas.push_back({b0, b1, b2, b3});
    }
  }

  Vector2 calculateCenter(std::vector<Vector2> &points)
  {
    Vector2 centro(0, 0);
    for (const auto &p : points)
      centro = centro + p;
    return centro * (1.0f / points.size());
  }

  // Calcula uma escala segura para a curva interna
  std::vector<Vector2> internalScale(std::vector<Vector2> points)
  {
    const float minDistance = 500.0f;

    int N = points.size();
    Vector2 center = calculateCenter(points);
    float scale = 0.5f;
    bool scaleValid = false;
    std::vector<Vector2> innerPoints(N);

    while (!scaleValid && scale > 0.1f)
    {
      scaleValid = true;
      for (int i = 0; i < N; i++)
        innerPoints[i] = center + (points[i] - center) * scale;
      for (int i = 0; i < N && scaleValid; i++)
      {
        Vector2 A1 = innerPoints[i];
        Vector2 A2 = innerPoints[(i + 1) % N];

        for (int j = 0; j < N; j++)
        {
          Vector2 B1 = points[j];
          Vector2 B2 = points[(j + 1) % N];

          bool intersects = segmentsIntersect(A1, A2, B1, B2);
          bool distanciaInvalida = (A1 - A1).length() > minDistance;

          if (intersects || distanciaInvalida)
          {
            scale -= 0.05f;
            scaleValid = false;
            break;
          }
        }
      }
    }
    return (scale > 0.1f) ? innerPoints : std::vector<Vector2>{};
  }

  // Funcão para detectar colisão
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

  // Cria pista com base nos pontos
  void createCurves(std::vector<Vector2> &outerPoints)
  {
    const int N = outerPoints.size();
    if (N < 4)
      return; // Precisa de pelo menos 4 pontos para fazer curvas

    Vector2 center = calculateCenter(outerPoints);

    curvas.clear();
    addBezierCurves(outerPoints);
    std::vector<Vector2> innerPoints = internalScale(outerPoints);

    if (!innerPoints.empty())
      addBezierCurves(innerPoints);
    tank->origem = calculateSafePositon(outerPoints);
  }

  // Cria pista aleatória
  void createRandomCurves()
  {
    const int N = 6;
    const float radius = 300.0f;
    const float noise = 80.0f;
    const float stretchX = 1.5f;
    const float stretchY = 0.8f;

    std::uniform_real_distribution<float> dist(-noise, noise);
    std::vector<Vector2> outerPoints(N);
    std::vector<Vector2> innerPoints(N);
    // Calcula os pontos externos
    for (int i = 0; i < N; ++i)
    {
      float angle = 2 * PI * i / N;
      float r = radius + dist(*gen);
      float x = r * cos(angle) * stretchX;
      float y = r * sin(angle) * stretchY;
      outerPoints[i] = Vector2(x, y);
    }
    createCurves(outerPoints);
  }

  // Posiciona os barris/inimigos
  void createEnemies()
  {
    int created = 0;
    int attempts = 0;

    float minX = -1000.0f;
    float maxX = 1000.0f;
    float minY = -1000.0f;
    float maxY = 1000.0f;
    float minDistance = 100.0f; // distância mínima do tanque

    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    while (created < ENEMIES && attempts < ENEMIES * 50)
    {
      float x = distX(*gen);
      float y = distY(*gen);
      Vector2 point(x + tank->origem.x, y + tank->origem.y);
      float distanceToTank = (point - tank->origem).length();

      if (distanceToTank > minDistance && isPointInsideClosedArea(point) && !barrels->collideBarrel(point, point))
      {
        barrels->createBarrel(point);
        created++;
      }

      attempts++;
    }
  }

public:
  Map(Tank *tank, Barrels *barrels, std::mt19937 *gen) : tank(tank), barrels(barrels), gen(gen)
  {
    scale = 0.6;
    pScore = Vector2(-500 + tank->origem.x, -380 + tank->origem.y);
  }

  // Inicializa o mapa (modo normal ou edição)
  void init(int type, std::vector<Vector2> pointsInput)
  {
    if (type == ALTER_MAP_POINTS)
    {
      bool trackValid = isTrackValid(pointsInput);

      if (pointsInput.size() < 4)
      {
        printf("\nNao foi possivel criar pista. Por favor utilizar mais de 3 pontos.\n");
        createRandomCurves();
      }
      else if (!trackValid)
      {
        printf("\nNao foi possivel criar pista.\n");
        createRandomCurves();
      }
      else
        createCurves(pointsInput);
    }
    else
      createRandomCurves();
    createEnemies();
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

  // Renderiza tudo.
  void render()
  {
    drawCurves();
    drawScore();
    barrels->drawBarrels();
    tank->render();
  }

  // Verifica do projetil com o mapa
  void collideProj(Vector2 projectile)
  {
    if (tank->shoot && !isPointInsideClosedArea(projectile))
    {
      tank->setProjectil(false);
    }
  }

  // Verifica colisão do tank com o map
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
