
#ifndef __OBJETOS_3D__
#define __OBJETOS_3D__
// Classe que manipula os objetos 3D, a criação dos objetos através de malhas, rotação e transladação, diferentes perpectivas, e renderização do objeto.

#include "Vector2.h"
#include "Vector3.h"
#include "Bezier.h"
#include <string>
#define TEXT_POSITON_X1 -150
#define TEXT_POSITON_Y1 -150
#define TEXT_POSITON_X2 -150
#define TEXT_POSITON_Y2 -130
#define POSITON_Z 700
#define OFF_SET_CENTER 400
#define M_INIT 20
#define N_INIT 20
class Objetos3D
{
public:
  Bezier curve;
  Vector3 matriz[100][100];
  float angleX, angleY, angleZ;
  float posX, posY, posZ;
  int M, N;
  bool perspective;
  float heightPerTurn; // nova variável
  bool springMode;     // opcional, para alternar entre revolução comum e mola

  Vector2 center;

  // INIT do objeto 3D
  void set(Bezier curve, Vector2 center)
  {
    this->curve = curve;
    this->center = center;
    perspective = true;
    angleX = angleY = 0, angleZ = 0;
    posX = center.x + OFF_SET_CENTER;
    posY = 0;
    posZ = POSITON_Z;
    M = M_INIT;
    N = N_INIT;
    heightPerTurn = 5.0f;
    springMode = false;
  }

  // Preenche a malha com a curva de bezier dada pelos pontos;
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
      float deslocamentoVertical = springMode ? i * heightPerTurn : 0.0f;
      float ang = i * passo1;

      for (int j = 0; j <= N; j++)
      {
        Vector2 base = curve.evaluate((float)j / N);

        Vector3 p = Vector3(
            base.x * cos(ang),
            base.y + deslocamentoVertical,
            base.x * sin(ang));

        p = rotacionaX(p, angleX);
        p = rotacionaY(p, angleY);
        p = rotacionaZ(p, angleZ);
        p = translada(p, posX, posY, posZ);

        matriz[i][j] = p;
      }
    }
  }

  // Eventos do teclado para alterar a projeção do objeto 3D.
  void eventsKey(int key)
  {
    switch (key)
    {
    case 'A':
    case 'a':
      angleY -= 0.1f;
      break;
    case 'D':
    case 'd':
      angleY += 0.1f;
      break;
    case 'W':
    case 'w':
      angleX += 0.1f;
      break;
    case 'S':
    case 's':
      angleX -= 0.1f;
      break;
    case 'Q':
    case 'q':
      angleZ -= 0.1f;
      break;
    case 'E':
    case 'e':
      angleZ += 0.1f;
      break;
    case 'L':
    case 'l':
      posY += 10;
      break;
    case 'K':
    case 'k':
      posY -= 10;
      break;
    case 'P':
    case 'p':
      perspective = !perspective;
      break;
    case 'M':
    case 'm':
      springMode = !springMode;
      break;
    case 'Z':
    case 'z':
      heightPerTurn += 1.0f;
      break;
    case 'X':
    case 'x':
      if (heightPerTurn > 1.0f)
        heightPerTurn -= 1.0f;
      break;
    case '+':
      if (M < 99 || N < 99)
      {
        M++;
        N++;
      }
      break;
    case '-':
      if (M > 3)
        M--;
      if (N > 3)
        N--;
      break;
    }
    fillMesh();
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

  // Projeção: Perspectiva ou Orgografica
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
      float escala = 0.5f;
      p2.x = p.x * escala;
      p2.y = p.y * escala;
    }
    p2.x += center.x;
    p2.y += center.y;
    return p2;
  }

  // Desenha os triangulos para exibição de um objeto 3D
  void renderTriangle(Vector2 p1, Vector2 p2, Vector2 p3)
  {
    CV::line(p1, p2);
    CV::line(p2, p3);
    CV::line(p3, p1);
  }

  void drawControls()
  {
    char text[20];
    sprintf(text, "Resolucao: M=%d N: %d", M, N);
    CV::text(TEXT_POSITON_X1, TEXT_POSITON_Y1, perspective ? "Modo: Perspectiva" : "Modo: Ortografico");
    CV::text(TEXT_POSITON_X2, TEXT_POSITON_Y2, text);
  }

  // Renderiza o objeto 3D
  void render()
  {
    const int d = 500;
    CV::color(RED);
    for (int l = 0; l < M; l++)
    {
      for (int c = 0; c < N; c++)
      {
        Vector2 p1 = projeta(matriz[l][c], d, perspective);
        Vector2 p2 = projeta(matriz[l][c + 1], d, perspective);
        Vector2 p3 = projeta(matriz[l + 1][c], d, perspective);
        Vector2 p4 = projeta(matriz[l + 1][c + 1], d, perspective);
        renderTriangle(p1, p2, p4);
        renderTriangle(p1, p3, p4);
      }
    }
    drawControls();
  }
};

#endif