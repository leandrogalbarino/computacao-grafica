// Define uma classe para controle e cálculo de FPS (frames por segundo) e deltaTime;
// Para controle de FPS
#ifndef __FPS_H__
#define __FPS_H__

#include <GL/glut.h>

#include <chrono>
#include <iostream>

class Fps
{
public:
  std::chrono::high_resolution_clock::time_point lastTime;
  float timer;
  int current;
  int frames;

  Fps()
  {
    this->current = 0.0f;
    this->timer = 0.0f;
    this->frames = 0;
    this->lastTime = std::chrono::system_clock::now();
  }

  void control(float deltaTime)
  {
    timer += deltaTime;
    frames++;

    if (timer >= 1.0f)
    {
      current = frames;
      frames = 0;
      timer = 0.0f;
    }
  }

  float deltaTime()
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> delta = now - lastTime;
    lastTime = now;
    return delta.count();
  }
};

#endif