#include <stdlib.h>
Ponto avaliaSpline(float t, Ponto p1, Ponto p2, Ponto p3, Ponto p4)
{
}

bool intersecaoSegmentos(Ponto p1, Ponto p2, Ponto p3, Ponto p4)
{
}
bool intersecaoPontos(Ponto p1, Ponto p2)
{
}
bool bSplineIntersecao(Ponto v[])
{
  int N = 17;
  for (int i = 0; i <= N - 4; i++)
  {
    for (float t = 0; t <= 1; t += 0.00001)
    {
      Ponto pt1 = avaliaSpline(t, v[i], v[i + 1], v[i + 2], v[i + 3]);
      for (int j = i + 3; j <= N - 4; j++)
      {
        for (float t2 = 0; t2 <= 1; t2 += 0.00001)
        {
          Ponto pt2 = avaliaSpline(t2, v[j], v[j + 1], v[j + 2], v[j + 3]);
          if (intersecaoPontos(pt1, pt2))
            return true;
        }
      }
    }
  }
  return false;
}
typedef struct Ponto
{
  float x;
  float y;
};

typedef struct Segmento
{
  /* data */
  Ponto pt1;
  Ponto pt2;
};

void addSegmento(Ponto pt1, Ponto pt2)
{
}

int tamSegmentos()
{
}
Segmento segmentos[1000];

bool bSplineIntersecao(Ponto v[])
{
  int N = 17;
  const float passo = 0.01; // Quanto menor, mais preciso

  // Lista de segmentos da curva  // Gerar todos os segmentos da B-spline
  for (int i = 0; i <= N - 4; i++)
  {
    for (float t = 0; t < 1.0; t += passo)
    {
      Ponto pt1 = avaliaSpline(t, v[i], v[i + 1], v[i + 2], v[i + 3]);
      Ponto pt2 = avaliaSpline(t + passo, v[i], v[i + 1], v[i + 2], v[i + 3]);
      addSegmento(pt1, pt2);
    }
  }

  int numSegmentos = tamSegmentos();
  for (int i = 0; i < numSegmentos; i++)
  {
    for (int j = i + 2; j < numSegmentos; j++)
    {
      Ponto a1 = segmentos[i].pt1;
      Ponto a2 = segmentos[i].pt2;
      Ponto b1 = segmentos[j].pt1;
      Ponto b2 = segmentos[j].pt2;
      if (intersecaoSegmentos(a1, a2, b1, b2))
      {
        return true;
      }
    }
  }
  return false;
}

void pixel(float x, float y);

void criarCurva(Ponto v[])
{
  int N = v.length;
  for (int i = 0; i < N; i++)
  {
    for (float t = 0; t <= 1; t += 0.001)
    {
      Ponto p = avaliaSpline(t, v[i % N], v[(i + 1) % N], v[(i + 2) % N], v[(i + 3) % 4]);
      pixel(p.x, p.y);
    }
  }
}

void line(float x1, float y1, float x2, float y2); // já fornecida

Ponto avaliaBezier(float t, Ponto p0, Ponto p1, Ponto p2, Ponto p3)
{
  float u = 1 - t;
  float uu = u * u;
  float tt = t * t;
  float uuu = uu * u;
  float ttt = tt * t;

  Ponto ponto;
  ponto.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
  ponto.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
  return ponto;
}
Ponto derivadaBezier(float t, Ponto p0, Ponto p1, Ponto p2, Ponto p3);

void desenhaPerpendicular(Ponto p[])
{
  Ponto meio = avaliaBezier(0.5, p[0], p[1], p[2], p[3]);
  Ponto tangente = derivadaBezier(0.5, p[0], p[1], p[2], p[3]);
  float perp_x = -tangente.y;
  float perp_y = tangente.x;
  Ponto perp;
  perp.set(perp_x, perp_y);
  perp = perp.normalizar();

  float metade = 1.5;
  float x1 = meio.x - perp.x *metade;
  float y1 = meio.y - perp.y * metade;
  float x2 = meio.x + perp.x * metade;
  float y2 = meio.y + perp.y * metade;

  line(x1,y1,x2,y2);
}

bool verificaG1(Ponto v[], int N)
{
  for (int i = 0; i < N - 4; i += 3)
  {
    Ponto p2 = v[i + 2];
    Ponto p3 = v[i + 3];
    Ponto p1_prox = v[i + 4];

    float vx1 = p3 - p2;
    float vx2 = p1_prox - p3;

    float cross = produtoVetorial(vx1, vx2);
    if (fabs(cross) > 0.00001)
      return false;
    float len1 = vx1.comprimento();
    float len2 = vx1.comprimento();
    if (fabs(len1 - len2) > 0.00001)
      return false;
  }

  return true;
}

bool curvaInterseccao(Ponto v[])
{
  Array segmentos;
  float passo = 0.0001;
  for (int p = 0; p < 14; p++)
  {
    for (float t = 0; t <= 1; t += passo)
    {
      Ponto pt1 = avaliaSpline(t, v[p], v[p + 1], v[p + 2], v[p + 3]);
      Ponto pt2 = avaliaSpline(t + passo, v[p], v[p + 1], v[p + 2], v[p + 3]);
      segmentos.add(Segmento(pt1, pt2));
    }
  }

  for (int i = 0; i < segmentos.lenght; i++)
  {
    for (int j = i + 2; j < segmentos.size; j++)
    {
      if (intersecaoSegmentos(segmentos[i].p1, segmentos[i].p2, segmentos[j].p1, segmentos[j].p2))
      {
        return true;
      }
    }
  }
  return false;
}