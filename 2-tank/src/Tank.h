#ifndef __TANK_H__
#define __TANK_H__

#include "gl_canvas2d.h"
#include "Vector2.h"
#define WIDTH 75
#define HEIGHT 50
#define RADIUS 15

#define PROJECTILE_SIZE 10
#define PROJECTILE_SPEED 10

#define TANK_ROTATION_SPEED 1
#define TANK_SPEED 0.7
#define TANK_CANNON_SIZE 70

class Tank
{
public:
  bool turningRight;
  bool turningLeft;

  Vector2 dir, p2;
  Vector2 vectorTiro, vectorTank;
  Vector2 origem;
  Vector2 corners[4];
  Vector2 tankRect[4];

  Vector2 shootVector;
  Vector2 shootVectorDir;
  bool shoot;

  Tank()
  {
    origem = Vector2(300, 300);
    dir = Vector2(1, 0);
    vectorTank = Vector2(1, 0);
    p2 = dir;

    turningRight = false;
    turningLeft = false;
    shoot = false;

    corners[0] = Vector2(-WIDTH / 2, -HEIGHT / 2);
    corners[1] = Vector2(WIDTH / 2, -HEIGHT / 2);
    corners[2] = Vector2(WIDTH / 2, HEIGHT / 2);
    corners[3] = Vector2(-WIDTH / 2, HEIGHT / 2);
    for (int i = 0; i < 4; i++)
    {
      tankRect[i] = corners[i];
    }
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
    if (shoot)
    {
      return;
    }
    shoot = true;
    shootVector = origem; // <-- Começa da posição do tanque
    shootVectorDir = p2 - origem;
    shootVectorDir.normalize();
  }

  void projectil(float deltaTime)
  {

    if (!shoot)
    {
      return;
    }
    if (shootVector.x > 500 || shootVector.y > 500 || shootVector.x < -200 || shootVector.y < -200)
    {
      shoot = false;
      return;
    }
    shootVector.x += shootVectorDir.x * PROJECTILE_SPEED * deltaTime;
    shootVector.y += shootVectorDir.y * PROJECTILE_SPEED * deltaTime;
    CV::color(1, 0, 0);
    CV::circleFill(shootVector, 10, 20);
  }

  void tankUpdate(float deltaTime)
  {
    float theta = 0;

    if (turningLeft)
      theta = -TANK_ROTATION_SPEED * deltaTime;
    else if (turningRight)
      theta = TANK_ROTATION_SPEED * deltaTime;
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
    CV::color(1, 0, 0);
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
    origem.x += TANK_SPEED * dir.x;
    origem.y += TANK_SPEED * dir.y;
  }

  void render()
  {

    tankUpdate(0.01); // deltaTime
    vectorTiro = p2 - origem;
    vectorTank = dir;
    vectorTiro.normalize();
    vectorTank.normalize();
    projectil(1); // deltaTime

    CV::translate(origem.x, origem.y);
    CV::color(1, 0, 0);
    moveTank(0);
    tankDirection();

    drawVector(vectorTiro);
    CV::polygon(tankRect, 4);
    CV::circle(0, 0, RADIUS, 20);
  }
};

#endif