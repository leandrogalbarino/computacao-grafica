#ifndef __TANK_H__
#define __TANK_H__

#include "gl_canvas2d.h"
#include "Vector2.h"

class Tank
{
public:
  bool turningRight;
  bool turningLeft;
  // float x, y;
  float angle; // em radianos
  float speed;
  float rotationSpeed;
  Vector2 v1, v2;

  Tank(Vector2 v1, Vector2 v2)
  {
    v1 = v1;
    v2 = v2;
    turningRight = false;
    turningLeft = false;
  }

  void setTurningLeft(bool set)
  {
    turningLeft = set;
  }

  void setTurningRight(bool set)
  {
    turningRight = set;
  }

  void tankUpdate(float deltaTime)
  {
    if (turningLeft)
      angle -= rotationSpeed * deltaTime;
    if (turningRight)
      angle += rotationSpeed * deltaTime;

    v1.x += cos(angle) * speed * deltaTime;
    v1.y += sin(angle) * speed * deltaTime;
  }

  void drawTank()
  {
    CV::color(1, 1, 1); // Branco
    CV::rect(v1, v2);
    CV::circle((v1 / 2), 20, 20);
  }
};

#endif