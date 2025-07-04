/// Camera.h
/// Define a classe Camera, responsável por armazenar a posição, direção e movimentação da câmera na cena 3D.
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Coord.h"
#include <cmath>

#define CAM_SPEED 10
#define ROTATION_SPEED 5.0f
#define PI 3.14159265358979323846

class Camera
{
public:
  Coord coord;
  Coord dir;
  Coord ang;
  float rxSpeed;
  float rzSpeed;
  bool movingBackward;

  Coord up;
  float oppening;
  float znear;
  float zfar;
  float aspect;

  Camera()
  {
    rxSpeed = 0.0f;
    rzSpeed = 0.0f;
    coord.x = 0.0f;
    coord.y = 0.0f;
    coord.z = 15.0f;

    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = 1.0f;

    ang.x = 0.0f;
    ang.y = 0.0f;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    oppening = 20.0f;
    znear = 1;
    zfar = 500;
    aspect = 1;
    movingBackward = false;
  }

  void move(float deltaTime)
  {
    float direction = movingBackward ? -1.0f : 1.0f;
    coord.x += dir.x * CAM_SPEED * deltaTime * direction;
    coord.y += dir.y * CAM_SPEED * deltaTime * direction;
    coord.z += dir.z * CAM_SPEED * deltaTime * direction;
  }

  void updateVectorDir(float deltaTime)
  {
    ang.y += rxSpeed * deltaTime;
    ang.x += rzSpeed * deltaTime;

    if (ang.x > 89.0f)
      ang.x = 89.0f;
    if (ang.x < -89.0f)
      ang.x = -89.0f;

    float radY = ang.y * PI / 180.0f;
    float radX = ang.x * PI / 180.0f;

    dir.x = cos(radX) * sin(radY);
    dir.y = sin(radX);
    dir.z = cos(radX) * cos(radY);
  }

  void update(float deltaTime)
  {
    updateVectorDir(deltaTime);
    move(deltaTime);
  }
};

#endif