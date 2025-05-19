/*  Tank.h
  -------
  Este arquivo define a classe `Tank`, responsável por representar o tanque do jogador
  no jogo. A classe gerencia o movimento, rotação, disparo de projéteis, desenho na tela,
  detecção de colisões e exibição da barra de vida.

  O tanque se move em uma arena 2D usando vetores para direção e rotação, com suporte a
  entrada de teclado e mouse. Ele também possui um sistema de cooldown para colisões.
  */
#ifndef __TANK_H__
#define __TANK_H__

#include "gl_canvas2d.h"
#include "Vector2.h"
#include <ctime>
#include <cmath>

#define RED 2
#define GREEN 3
#define BLACK 0

#define WIDTH 50
#define HEIGHT 25
#define RADIUS 7

#define PROJECTILE_SIZE 5

#define PROJECTILE_SPEED 800
#define TANK_ROTATION_SPEED 1
#define TANK_SPEED 90

#define TANK_CANNON_SIZE 50
#define MAX_LIFE 200
#define COLLISION_COOLDOWN_MS 500

class Tank
{
public:
  // Estado de rotação
  bool turningRight;
  bool turningLeft;

  int life;
  int score;

  Vector2 dir, p2;    // dir: direção do tanque; p2: posição do mouse (alvo do canhão)
  Vector2 vectorTiro; // Vetores normalizado de tiro
  Vector2 origem;     // Posição do centro do tanque

  Vector2 corners[4];  // Cantos originais
  Vector2 tankRect[4]; // Cantos transformados (após rotação)

  clock_t lastFrame;     // Para calcular deltaTime
  clock_t lastCollision; // Controle de cooldown de colisão

  // Vetores de controle do projétil
  Vector2 shootVectorNew;
  Vector2 shootVectorOld;
  Vector2 shootVectorDir;
  bool shoot; // estado de disparo

  Tank()
  {
    origem = Vector2(600, 400);
    dir = Vector2(-std::cos(PI / 4), std::sin(PI / 4));
    p2 = dir;
    turningRight = false;
    turningLeft = false;
    shoot = false;
    life = MAX_LIFE;
    score = 0;
    lastCollision = 0;
    lastFrame = clock();
    corners[0] = Vector2(-WIDTH / 2, -HEIGHT / 2);
    corners[1] = Vector2(WIDTH / 2, -HEIGHT / 2);
    corners[2] = Vector2(WIDTH / 2, HEIGHT / 2);
    corners[3] = Vector2(-WIDTH / 2, HEIGHT / 2);
    for (int i = 0; i < 4; i++)
      tankRect[i] = corners[i];
  }

  void setTurningLeft(bool value)
  {
    turningLeft = value;
  }

  void setTurningRight(bool value)
  {
    turningRight = value;
  }

  // Define o vetor de direção do canhão com base no mouse
  void setMousePositon(float x, float y)
  {
    p2.set(x, y);
  }

  // Realiza outro disparo, apenas quando não tem um outro disparo em andamento.
  void setProjectil(bool value)
  {
    if (shoot && value)
      return;

    shoot = value;
    if (value)
    {
      shootVectorNew = origem; // <-- Começa da posição do tanque
      shootVectorOld = origem;
      shootVectorDir = p2 - origem;
      shootVectorDir.normalize();
    }
  }

  // Atualiza o estado de colisão e aplica dano se necessário
  void UpdateCollision(int life, clock_t now)
  {
    this->life -= life;
    dir = dir * -1;
    lastCollision = now;
  }

  // Calcula e desenha a trajetoria do disparo.
  void projectil(float deltaTime)
  {
    if (!shoot)
      return;

    shootVectorOld = shootVectorNew;
    shootVectorNew.x += shootVectorDir.x * PROJECTILE_SPEED * deltaTime;
    shootVectorNew.y += shootVectorDir.y * PROJECTILE_SPEED * deltaTime;
    CV::color(RED);
    CV::circleFill(shootVectorNew, 5, 20);
  }

  // Atualiza o vetor direcão;
  void tankUpdate(float deltaTime)
  {
    float theta = 0;

    if (turningLeft)
      theta = +TANK_ROTATION_SPEED * deltaTime;
    else if (turningRight)
      theta = -TANK_ROTATION_SPEED * deltaTime;
    else
      return;

    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    float x = dir.x;
    float y = dir.y;

    dir.x = x * cosTheta - y * sinTheta;
    dir.y = x * sinTheta + y * cosTheta;
  }

  void drawVector(Vector2 v)
  {
    CV::color(RED);
    Vector2 vectorEsc = v * TANK_CANNON_SIZE;
    CV::line(0, 0, vectorEsc.x, vectorEsc.y);
  }

  // Calcula a rotacao com base no vetor direcao
  void tankDirection()
  {
    Vector2 perp(-dir.y, dir.x);
    for (int i = 0; i < 4; i++)
    {
      float x = corners[i].x;
      float y = corners[i].y;

      tankRect[i].x = x * dir.x + y * perp.x;
      tankRect[i].y = x * dir.y + y * perp.y;
    }
  }

  void moveTank(float deltaTime)
  {
    origem.x += TANK_SPEED * dir.x * deltaTime;
    origem.y += TANK_SPEED * dir.y * deltaTime;
  }

  void drawLife()
  {
    if (life <= 0)
    {
      return;
    }

    float barWidth = WIDTH / 1.0f / MAX_LIFE;
    float startX = -WIDTH / 2.0f;
    float endX = WIDTH / 2.0f;

    for (int i = 0; i < MAX_LIFE; i++)
    {
      // 3 - verde, 2 vermelho
      CV::color(i <= life ? GREEN : RED);
      float x1 = startX + i * barWidth;
      float x2 = endX;
      float y1 = -HEIGHT - 7;
      float y2 = -HEIGHT - 2;
      CV::rectFill(x1, y1, x2, y2);
    }
  }

  void render()
  {
    // Controle de fps
    clock_t now = clock();
    float deltaTime = float(now - lastFrame) / CLOCKS_PER_SEC;
    lastFrame = now;

    tankUpdate(deltaTime);
    vectorTiro = p2 - origem;
    vectorTiro.normalize();
    dir.normalize();
    projectil(deltaTime);

    CV::translate(origem.x, origem.y);
    drawLife();
    CV::color(RED);
    moveTank(deltaTime);
    tankDirection();

    drawVector(vectorTiro);
    CV::polygon(tankRect, 4);
    CV::circle(0, 0, RADIUS, 20);
  }
};

#endif
