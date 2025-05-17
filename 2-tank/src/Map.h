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

#define DANO_TANK 20
#define ENEMIES 3

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
public:
  std::vector<BezierCurve> curvas;
  Tank *tank;
  Barrels *barrels;
  std::mt19937 *gen;
  Vector2 pScore;
  float esc;

  Map(Tank *tank, Barrels *barrels, std::mt19937 *gen) : tank(tank), barrels(barrels), gen(gen)
  {
    createCurvas();
    createEnemies();
    esc = 0.6;
    // posição do SCORE não muda.
    pScore = Vector2(-500 + tank->origem.x, -320 + tank->origem.y);
  }

void createCurvas()
{
    // Pontos relativos originais (curva externa)
    Vector2 points[] = {
            Vector2(-300, 0),
            Vector2(-20, 250),
            Vector2(150, 250),
            Vector2(300, 0),
            Vector2(150, -250),
            Vector2(-150, -250)
        };

    // Calcula o centro geométrico
    Vector2 centro(0, 0);
    for (const auto &p : points)
    {
        centro = centro + p;
    }
    centro = centro * (1.0f / 6);

    // Cria curvas externas (original)
    Vector2 p0 = points[0] - centro + tank->origem;
    Vector2 p1 = points[1] - centro + tank->origem;
    Vector2 p2 = points[2] - centro + tank->origem;
    Vector2 p3 = points[3] - centro + tank->origem;
    Vector2 p4 = points[4] - centro + tank->origem;
    Vector2 p5 = points[5] - centro + tank->origem;

    // Limpa curvas existentes e adiciona as externas
    curvas.clear();
    curvas.push_back({p0, p1, p2, p3});
    curvas.push_back({p3, p4, p5, p0});

    // Cria curvas internas com verificação de distância
    float minDistance = 200.0f;  // Aumentei a distância mínima para garantir
    esc = 0.6f;         // Começa com escala maior
    
    // Encontra a escala segura
    bool escalaValida = false;
    while (!escalaValida && esc > 0.1f)  // Limite mínimo para evitar escala muito pequena
    {
        escalaValida = true;
        for (int i = 0; i < 6; i++)
        {
            Vector2 pontoInterno = centro + (points[i] - centro) * esc;
            Vector2 pontoExterno = points[i];
            
            // Verifica distância entre os pontos correspondentes
            if ((pontoInterno - pontoExterno).length() < minDistance)
            {
                esc -= 0.05f;
                escalaValida = false;
                break;
            }
        }
    }

    // Cria pontos internos com a escala ajustada
    Vector2 ip[6];
    for (int i = 0; i < 6; i++)
    {
        ip[i] = centro + (points[i] - centro) * esc + (tank->origem - centro);
    }

    // Adiciona curvas internas
    curvas.push_back({ip[0], ip[1], ip[2], ip[3]});
    curvas.push_back({ip[3], ip[4], ip[5], ip[0]});

    // Posiciona o tanque em uma área segura (no meio do caminho)
    float escalaSegura = (1.0f + esc) * 0.5f;  // Ponto médio entre as curvas
    Vector2 safePosition = centro + (points[0] - centro) * escalaSegura + (tank->origem - centro);
    tank->origem = safePosition;
}

  void createEnemies()
  {
    int criados = 0;
    int tentativas = 0;

    // Região de geração baseada em uma bounding box "razoável"
    float minX = -500.0f;
    float maxX = 500.0f;
    float minY = -500.0f;
    float maxY = 500.0f;

    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);

    while (criados < ENEMIES && tentativas < ENEMIES * 50)
    {
      float x = distX(*gen);
      float y = distY(*gen);
      Vector2 ponto(x + tank->origem.x, y + tank->origem.y);

      if (isPointInsideClosedArea(ponto) && !barrels->collideBarrel(ponto, ponto))
      {
        barrels->createBarrel(ponto);
        criados++;
      }

      tentativas++;
    }
  }

  void drawScore()
  {
    char scoreText[50];
    sprintf(scoreText, "Pontuacao: %d", tank->score);
    CV::color(BLACK);
    CV::text(pScore, scoreText);
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

        // Verifica se o raio horizontal para a direita cruza este segmento
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

    // Se o número de interseções for ímpar, o ponto está dentro
    return (intersections % 2) == 1;
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
  void render()
  {
    drawCurves();
    drawScore();
    barrels->drawBarrels();
    tank->render();
  }
  // OK
  void collideProj(Vector2 projectile)
  {
    if (!isPointInsideClosedArea(projectile))
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

      if (!isPointInsideClosedArea(pos))
      {
        tank->life -= DANO_TANK;
        tank->dir = tank->dir * -1;
        break;
      }
    }
  }
};

#endif
