
#ifndef __OBJETOS_3D__
#define __OBJETOS_3D__

#include "Vector2.h"
#include "Vector3.h"
#include "Bezier.h"
#include <string>
class Objetos3D
{
public:
  Bezier curve;
  Vector3 matriz[100][100];
  float angleX, angleY, angleZ;
  float posX, posY, posZ;
  int M, N;
  bool perspectiva;
  Vector2 center;

  void set(Bezier curve, Vector2 center)
  {
    this->curve = curve;
    this->center = center;
    angleX = angleY = 0, angleZ = 0;
    posX = 1280 / 2 + 200;
    posY = 0;
    posZ = 700;
    M = 20;
    N = 20;
    perspectiva = true;
  }

  void fillMesh()
  {
    float passo1 = 2 * PI / M;

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
        Vector3 p = Vector3(
            perfil2D[j].x * cos(ang),
            perfil2D[j].y * 1.8f,
            perfil2D[j].x * sin(ang));

        p = rotacionaX(p, angleX);
        p = rotacionaY(p, angleY);
        p = rotacionaZ(p, angleZ);
        p = translada(p, posX, posY, posZ);

        matriz[i][j] = p;
      }
    }
  }

  Vector3 rotacionaZ(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.x = pAux.x * cos(ang) - pAux.y * sin(ang);
    p.y = pAux.x * sin(ang) + pAux.y * cos(ang);
    return p;
  }

  Vector3 rotacionaX(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.y = pAux.y * cos(ang) - pAux.z * sin(ang);
    p.z = pAux.y * sin(ang) + pAux.z * cos(ang);

    return p;
  }

  Vector3 rotacionaY(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.x = pAux.x * cos(ang) + pAux.z * sin(ang);
    p.z = -pAux.x * sin(ang) + pAux.z * cos(ang);
    return p;
  }

  Vector3 translada(Vector3 p, float tx, float ty, float tz)
  {
    return Vector3(p.x + tx, p.y + ty, p.z + tz);
  }

  Vector3 transladaZ(Vector3 p, int translate)
  {
    p.z = p.z + translate;
    return p;
  }

  Vector2 projeta(Vector3 p, int d, bool perspectiva)
  {
    Vector2 p2;
    if (perspectiva)
    {
      p2.x = p.x * d / (p.z + d);
      p2.y = p.y * d / (p.z + d);
    }
    else
    {
      float ortographicScale = 1.0f + (p.z / 1000.0f); // Fator de escala baseado em Z
      p2.x = (p.x - 50) / ortographicScale;           // Diminui X conforme Z aumenta
      p2.y = p.y / ortographicScale;                   // Diminui Y conforme Z aumenta
    }
    p2.x += center.x;
    p2.y += center.y;
    return p2;
  }

  void renderTriangle(Vector2 p1, Vector2 p2, Vector2 p3)
  {
    CV::line(p1, p2);
    CV::line(p2, p3);
    CV::line(p3, p1);
    // Poderia adicionar preenchimento aqui
  }

  Vector3 escala(Vector3 p, int escala)
  {
    p = p * escala;
    return p;
  }

  void render()
  {
    const int d = 500; 
    CV::color(RED);
    for (int l = 0; l < M; l++)
    {
      for (int c = 0; c < N; c++)
      {
        Vector2 p1 = projeta(matriz[l][c], d, perspectiva);
        Vector2 p2 = projeta(matriz[l][c + 1], d, perspectiva);
        Vector2 p3 = projeta(matriz[l + 1][c], d, perspectiva);
        Vector2 p4 = projeta(matriz[l + 1][c + 1], d, perspectiva);

        renderTriangle(p1, p2, p4);
        renderTriangle(p1, p3, p4);
      }
    }
    CV::text(-150, -150, perspectiva ? "Modo: Perspectiva" : "Modo: Ortografico");
    char text[20];
    sprintf(text, "Resolucao: M=%d N: %d", M, N);
    CV::text(-150, -130, text);
  }
};

#endif