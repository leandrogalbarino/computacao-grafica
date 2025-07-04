/// Map.h
/// Define a classe Map, responsável por gerenciar e renderizar os objetos da cena, incluindo estrelas, asteroides, e a fonte de luz (sol).

#ifndef __MAP_H__
#define __MAP_H__

#include <GL/glut.h>

#include <vector>
#include <cstdlib>
#include <cmath>
#include <chrono>

#include "Coord.h"
#include "Camera.h"
#include "Fps.h"

#define SCREEN_X 1280
#define SCREEN_Y 720
#define STAR_SIZE 2.0f
#define ASTEROIDS_SIZE 5.0f
#define REMOVE_DISTANCE 300.0f
#define NUM_STARS 300
#define NUM_ASTEROIDS 300

class Map
{
private:
  void drawText(float x, float y, const char *text)
  {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_X, 0, SCREEN_Y);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
  }

  Coord createSphere(float distanceFront, float side, float vertical)
  {
    Coord s;
    float baseX = cam->coord.x + cam->dir.x * distanceFront;
    float baseY = cam->coord.y + cam->dir.y * distanceFront;
    float baseZ = cam->coord.z + cam->dir.z * distanceFront;

    float offsetX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * side;
    float offsetY = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * vertical;
    float offsetZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * side;

    s.x = baseX + offsetX;
    s.y = baseY + offsetY;
    s.z = baseZ + offsetZ;
    return s;
  }

public:
  std::vector<Coord> asteroids;
  std::vector<Coord> stars;
  Coord sun;
  Camera *cam;
  Fps *fps;
  bool wireframeMode;
  std::chrono::high_resolution_clock::time_point lastAsteroidTime;

  Map()
  {
    sun.x = -10.0f;
    sun.y = 0.0f;
    sun.z = 100.0f;
    wireframeMode = false;
  }

  // Inicia nosso jogo criando objetos.
  void init(Fps *fps, Camera *cam)
  {
    this->fps = fps;
    this->cam = cam;
    createAsteroids();
    createStars();
  }

  void createAsteroids()
  {
    for (int i = 0; i < NUM_ASTEROIDS; i++)
    {
      float distanceFront = 10.0f + static_cast<float>(rand()) / RAND_MAX * (60.0f - 10.0f);
      float side = 50.0f;
      float vertical = 30.0f;
      Coord s = createSphere(distanceFront, side, vertical);
      asteroids.push_back(s);
    }
  }

  void createStars()
  {
    for (int i = 0; i < NUM_STARS; i++)
    {
      float distanceFront = 30.0f + static_cast<float>(rand()) / RAND_MAX * (100.0f - 30.0f);
      float side = 50.0f;
      float vertical = 50.0f;

      Coord s = createSphere(distanceFront, side, vertical);
      stars.push_back(s);
    }
  }

  void removeLastAsteroids()
  {
    for (int i = 0; i < asteroids.size();)
    {
      float dx = asteroids[i].x - cam->coord.x;
      float dy = asteroids[i].y - cam->coord.y;
      float dz = asteroids[i].z - cam->coord.z;
      float dist = sqrt(dx * dx + dy * dy + dz * dz);

      if (dist > REMOVE_DISTANCE)
      {
        asteroids.erase(asteroids.begin() + i);
      }
      else
      {
        i++;
      }
    }
  }

  void updateAsteroids()
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> delta = now - lastAsteroidTime;

    if (delta.count() >= 0.2f)
    {
      lastAsteroidTime = now;
      Coord asteroid;
      float newDistance = 100.0f;

      float randX = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
      float randY = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
      float randZ = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;

      asteroid.x = cam->coord.x + cam->dir.x * newDistance + randX;
      asteroid.y = cam->coord.y + cam->dir.y * newDistance + randY;
      asteroid.z = cam->coord.z + cam->dir.z * newDistance + randZ;
      asteroids.push_back(asteroid);
    }
  }

  void updateStars()
  {
    for (Coord &star : stars)
    {
      float dx = star.x - cam->coord.x;
      float dy = star.y - cam->coord.y;
      float dz = star.z - cam->coord.z;

      float distance = sqrt(dx * dx + dy * dy + dz * dz);

      if (distance < 90.0f)
      {
        float newDistance = 100.0f;

        float randX = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
        float randY = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
        float randZ = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;

        star.x = cam->coord.x + cam->dir.x * newDistance + randX;
        star.y = cam->coord.y + cam->dir.y * newDistance + randY;
        star.z = cam->coord.z + cam->dir.z * newDistance + randZ;
      }
    }
  }

  void drawInfo()
  {
    char fpsText[64];
    char angY[20];
    char angX[20];
    sprintf(angX, "ANG X: %f", cam->ang.x);
    sprintf(angY, "ANG Y: %f", cam->ang.y);

    sprintf(fpsText, "FPS: %d", fps->current);
    drawText(10, SCREEN_Y - 20, fpsText);
    drawText(10, SCREEN_Y - 40, "W,A,S,D - MOVIMENTO");
    drawText(10, SCREEN_Y - 60, "P - MODO WIFRAME");
    drawText(10, SCREEN_Y - 80, angX);
    drawText(10, SCREEN_Y - 100, angY);
  }

  void drawStars()
  {
    glPointSize(STAR_SIZE);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (const Coord &s : stars)
    {
      glVertex3f(s.x, s.y, s.z);
    }
    glEnd();
  }

  void drawAsteroids()
  {
    for (const Coord &s : asteroids)
    {
      glPushMatrix();
      glColor3f(0.7, 0.3, 0.3); // Cor da esfera
      glTranslatef(s.x, s.y, s.z);
      glutSolidSphere(0.3, 16, 16);
      glPopMatrix();
    }
  }

  void drawSun()
  {
    glPushMatrix();
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(sun.x, sun.y, sun.z);
    glutSolidSphere(3.0, 32, 32);

    glEnable(GL_LIGHTING);
    glPopMatrix();
  }

  void ilumination()
  {
    Coord lightDirection = Coord(-sun.x, -sun.y, -sun.z);

    GLfloat light_pos[] = {lightDirection.x, lightDirection.y, lightDirection.z, 0.0f};
    GLfloat light_amb[] = {0.05f, 0.05f, 0.05f, 1.0f};
    GLfloat light_dif[] = {0.9f, 0.6f, 0.5f, 1.0f};    
    GLfloat light_spe[] = {0.8f, 0.8f, 0.8f, 1.0f};   

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);

    GLfloat mat_specular[] = {0.9f, 0.9f, 0.9f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
  }


  void alterMode()
  {
    wireframeMode = !wireframeMode;
  }

  // Modo de visuliazação dos objetos 3D
  int modeView()
  {
    return wireframeMode ? GL_LINE : GL_FILL;
  }

  // Atualização
  void update()
  {
    updateStars();
    updateAsteroids();
    removeLastAsteroids();
  }

  // Desenha
  void render()
  {
    ilumination();
    drawSun();
    drawStars();
    drawAsteroids();
    drawInfo();
  }
};

#endif