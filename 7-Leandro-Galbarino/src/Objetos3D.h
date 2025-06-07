
#ifndef __OBJETOS_3D__
#define __OBJETOS_3D__

#include "Vector2.h"
#include "Vector3.h"
#include "Bezier.h"
class Objetos3D
{
public:
  Bezier curve;
  Vector3 matriz[100][100];

  void set(Bezier curve){
    this->curve = curve;
  }

  void fillMesh()
  {
    int M = 20;                // Divisões horizontais (azimutal)
    float passo1 = 2 * PI / M; // angulo azimutal

    const int N = 20;
    Vector2 perfil2D[N + 1];
    for (int j = 0; j <= N; j++)
    {
      float t = (float)j / N;
      perfil2D[j] = curve.evaluate(t);
    }

    for (int i = 0; i <= M; i++)
    {
      float ang = i * passo1;
      for (int j = 0; j <= N; j++)
      {
        float x = perfil2D[j].x * cos(ang);
        float z = perfil2D[j].x * sin(ang);
        float y = perfil2D[j].y;
        matriz[i][j] = Vector3(x, y, z + 1000);
      }
    }
  }

  Vector3 rotacionaY(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.x = pAux.x * cos(ang) + pAux.z * sin(ang);
    p.z = (pAux.x) * -1 * sin(ang) + pAux.z * cos(ang);
    return p;
  }

  Vector3 transladaZ(Vector3 p, int translate)
  {
    p.z = p.z + translate;
    return p;
  }

  Vector2 projeta(Vector3 p, int d)
  {
    Vector2 p2;
    p2.x = p.x * d / p.z;
    p2.y = p.y * d / p.z;
    return p2;
  }

  Vector3 escala(Vector3 p, int escala)
  {
    p = p * escala;
    return p;
  }

  void render()
  {
    CV::color(RED);
    const int M = 20; // Evita acessar fora da matriz
    const int N = 20;
    const int d = 400; // Distância da câmera para projeção (pode ajustar depois)
    for (int l = 0; l < M; l++)
    {
      for (int c = 0; c < N; c++)
      {
        Vector2 p1 = projeta(matriz[l][c], d);
        Vector2 p2 = projeta(matriz[l][c + 1], d);
        Vector2 p3 = projeta(matriz[l + 1][c], d);
        Vector2 p4 = projeta(matriz[l + 1][c + 1], d);
        CV::line(p1, p2);
        CV::line(p1, p3);
        CV::line(p1, p4);
      }
    }
  }
};

#endif