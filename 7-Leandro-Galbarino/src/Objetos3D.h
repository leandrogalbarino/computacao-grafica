
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

  void set(Bezier curve)
  {
    this->curve = curve;
    angleX = angleY = 0, angleZ = 0;
    posX =  1280/2 + 200;
    posY = 0;
    posZ = 1000;
    M = 20;
    N = 20;
    perspectiva = true;
  }

  void fillMesh()
  {
    float passo1 = 2 * PI / M; // angulo azimutal

    Vector2 perfil2D[N + 1];
    for (int j = 0; j <= N; j++)
    {
      float t = (float)j / N;
      perfil2D[j] = curve.evaluate(t);
    }

    for (int i = 0; i <= M; i++)
    {

      float ang = i * passo1 + angleY;
      for (int j = 0; j <= N; j++)
      {
        Vector3 p = Vector3(
            perfil2D[j].x * cos(ang),
            perfil2D[j].y,
            perfil2D[j].x * sin(ang));
        p = rotacionaX(p, angleX);
        p = rotacionaZ(p, angleZ);
        p = translada(p, posX, posY, posZ); // Nova translação
        matriz[i][j] = p;
      }
    }
  }

  Vector3 rotacionaZ(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.x = pAux.x * cos(ang) - pAux.y * sin(ang);
    p.y = pAux.x * sin(ang) + pAux.y * cos(ang);
    // z permanece o mesmo
    return p;
  }

  Vector3 rotacionaX(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.y = pAux.y * cos(ang) + pAux.z * sin(ang);
    p.z = (pAux.y) * -1 * sin(ang) + pAux.z * cos(ang);
    return p;
  }

  Vector3 rotacionaY(Vector3 p, float ang)
  {
    Vector3 pAux = p;
    p.x = pAux.x * cos(ang) + pAux.z * sin(ang);
    p.z = (pAux.x) * -1 * sin(ang) + pAux.z * cos(ang);
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
      p2.x = p.x * d / p.z;
      p2.y = p.y * d / p.z;
    }
    else
    {
      p2.x = p.x;
      p2.y = p.y;
      p.z = p.z + 400;
    }
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
    const int d = 400; // Distância da câmera para projeção (pode ajustar depois)
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
    CV::text(10, 20, perspectiva ? "Modo: Perspectiva" : "Modo: Ortografico");
    char text[20];
    sprintf(text, "Resolucao: M=%d N: %d", M, N);
    CV::text(10, 40, text);
  }
};

#endif