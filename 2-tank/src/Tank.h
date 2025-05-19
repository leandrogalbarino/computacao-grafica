#ifndef __TANK_H__
#define __TANK_H__

#include "gl_canvas2d.h"
#include "Vector2.h"
#include <ctime>

#define RED 2
#define GREEN 3
#define BLACK 0

#define WIDTH 50
#define HEIGHT 25
#define RADIUS 7

#define PROJECTILE_SIZE 5

#define PROJECTILE_SPEED 900
#define TANK_ROTATION_SPEED 1
#define TANK_SPEED 90

#define TANK_CANNON_SIZE 50
#define MAX_LIFE 200
#define COLLISION_COOLDOWN_MS 500

class Tank
{
public:
  bool turningRight;
  bool turningLeft;
  int life;
  int score;
  Vector2 dir, p2;
  Vector2 vectorTiro, vectorTank;
  Vector2 origem;
  Vector2 corners[4];
  Vector2 tankRect[4];
  clock_t lastFrame;
  Vector2 shootVectorNew;
  Vector2 shootVectorOld;
  Vector2 shootVectorDir;
  clock_t lastCollision;
  bool shoot;

  Tank()
  {
    origem = Vector2(600, 400);
    dir = Vector2(-std::cos(M_PI / 4), std::sin(M_PI / 4));
    vectorTank = Vector2(1, 0);
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

  void setMousePositon(float x, float y)
  {
    p2.set(x, y);
  }

  void setProjectil(bool value)
  {
    if (shoot && value)
    {
      return;
    }

    shoot = value;
    if (value)
    {
      shootVectorNew = origem; // <-- Começa da posição do tanque
      shootVectorOld = origem; // <-- Começa da posição do tanque
      shootVectorDir = p2 - origem;
      shootVectorDir.normalize();
    }
  }
  void UpdateCollision(int life, clock_t now)
  {
    this->life -= life;
    dir = dir * -1;
    lastCollision = now;
  }

  void projectil(float deltaTime)
  {

    if (!shoot)
    {
      return;
    }
    shootVectorOld = shootVectorNew;
    shootVectorNew.x += shootVectorDir.x * PROJECTILE_SPEED * deltaTime;
    shootVectorNew.y += shootVectorDir.y * PROJECTILE_SPEED * deltaTime;
    CV::color(RED);
    CV::circleFill(shootVectorNew, 5, 20);
  }

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

  void tankDirection()
  {
    Vector2 perp(-vectorTank.y, vectorTank.x);
    for (int i = 0; i < 4; i++)
    {
      float x = corners[i].x;
      float y = corners[i].y;

      tankRect[i].x = x * vectorTank.x + y * perp.x;
      tankRect[i].y = x * vectorTank.y + y * perp.y;
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
    // Calcular1 deltaTime  FRAMES POR SEC
    clock_t now = clock();
    float deltaTime = float(now - lastFrame) / CLOCKS_PER_SEC;
    lastFrame = now;

    tankUpdate(deltaTime); // deltaTime
    vectorTiro = p2 - origem;
    vectorTank = dir;
    vectorTiro.normalize();
    vectorTank.normalize();
    projectil(deltaTime); // deltaTime

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