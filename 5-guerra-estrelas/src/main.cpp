// • A aplicação não necessita ter botões ou interfaces.
// • A animação da câmera deve ser suave (não usar refresh do teclado).

// • O código deve estar bem estruturado em C++ e deve fazer uso de vetores e
// transformações geométricas.
// • A movimentação da câmera deve ser feita obrigatoriamente com vetores
// (direção, posição), caso contrario o trabalho não será aceito.

// • Todos os modelos 3D devem ter vetores normais. Deve-se obrigatoriamente
// incluir alguma iluminação na cena.

// • Controle de FPS

// • Os asteroides podem ser modelados como esferas ou outros modelos disponíveis

// na glut (esfera, torus, cubo, bule etc.), ou modelos mais complexos.

// • Visualização em wireframe e com preenchimento de polígonos.

// • O trabalho deve ser apresentado individualmente.

#include <GL/glut.h>

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <vector>
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include <cmath>
#include <chrono>
using namespace std::chrono;
int frames = 0;

#define SCREEN_X 1280
#define SCREEN_Y 720
#define NUM_STARS 300
#define STAR_SIZE 2.0f
#define NUM_SPHERES 300
#define ROTATION_SPEED 7.0f;
#define REMOVE_DISTANCE 300.0f
bool polygonMode;

float rx = 0, rz = 0;

float abertura = 20.0;
float znear = 1;
float zfar = 500;
float aspect = 1;
float camX = 0.0f, camY = 0.0f, camZ = 15.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = 1.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
float rxSpeed = 0.0f;
float rzSpeed = 0.0f;
float camSpeed = 10;
float angleY = 0.0f; // Rotação em torno do eixo Y (horizontal)
float angleX = 0.0f; // Rotação em torno do eixo X (vertical)
int currentFPS = 0;
bool movingBackward = false;

steady_clock::time_point lastAsteroidTime = steady_clock::now();
auto lastFrameTime = high_resolution_clock::now();

float sunX = 5.0f;
float sunY = 0.0f;
float sunZ = 60.0f;

struct Sphere
{
   float x, y, z;
};

std::vector<Sphere> asteroids;
std::vector<Sphere> stars;

void drawStars()
{
   glPointSize(STAR_SIZE);
   glBegin(GL_POINTS);
   glColor3f(1.0f, 1.0f, 1.0f);
   for (const Sphere &s : stars)
   {
      glVertex3f(s.x, s.y, s.z);
   }
   glEnd();
}

void drawAsteroids()
{
   for (const Sphere &s : asteroids)
   {
      glPushMatrix();
      glColor3f(0.7, 0.3, 0.3); // Cor da esfera
      glTranslatef(s.x, s.y, s.z);
      glutSolidSphere(0.5, 16, 16); // Raio 0.5, 16 slices e stacks
      glPopMatrix();
   }
}

void drawSun()
{
   glPushMatrix();
   glDisable(GL_LIGHTING);

   glColor3f(1.0f, 1.0f, 0.0f);
   glTranslatef(sunX, sunY, sunZ);
   glutSolidSphere(3.0, 256, 256);

   glEnable(GL_LIGHTING);
   glPopMatrix();
}
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

void drawInfo()
{
   char fpsText[64];
   char angY[20];
   char angX[20];
   sprintf(angX, "ANG X: %f", angleX);
   sprintf(angY, "ANG Y: %f", angleY);

   sprintf(fpsText, "FPS: %d", currentFPS);
   drawText(10, SCREEN_Y - 20, fpsText);
   drawText(10, SCREEN_Y - 40, "W,A,S,D - MOVIMENTO");
   drawText(10, SCREEN_Y - 60, "P - MODO WIFRAME");
   drawText(10, SCREEN_Y - 80, angX);
   drawText(10, SCREEN_Y - 100, angY);
}

void ilumination()
{
   GLfloat light_pos[] = {sunX, sunY, sunZ, 1.0f}; // luz pontual
   GLfloat light_amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
   GLfloat light_dif[] = {1.0f, 1.0f, 0.8f, 1.0f};
   GLfloat light_spe[] = {1.0f, 1.0f, 0.8f, 1.0f};

   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);
}

void render(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, aspect, znear, zfar);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt(camX, camY, camZ,
             camX + dirX, camY + dirY, camZ + dirZ,
             upX, upY, upZ);

   ilumination();
   drawSun();
   drawStars();
   drawAsteroids();
   drawInfo();

   glutSwapBuffers();
}

Sphere createSphere(float distanceFront, float side, float vertical)
{
   Sphere s;
   float baseX = camX + dirX * distanceFront;
   float baseY = camY + dirY * distanceFront;
   float baseZ = camZ + dirZ * distanceFront;

   float offsetX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * side;
   float offsetY = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * vertical;
   float offsetZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * side;

   s.x = baseX + offsetX;
   s.y = baseY + offsetY;
   s.z = baseZ + offsetZ;
   return s;
}

void createAsteroids()
{
   for (int i = 0; i < NUM_SPHERES; i++)
   {
      float distanceFront = 10.0f + static_cast<float>(rand()) / RAND_MAX * (60.0f - 10.0f);
      float side = 50.0f;
      float vertical = 30.0f;
      Sphere s = createSphere(distanceFront, side, vertical);
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

      Sphere s = createSphere(distanceFront, side, vertical);
      stars.push_back(s);
   }
}
int mode()
{
   return polygonMode ? GL_LINE : GL_FILL;
}
void onInit()
{
   srand(time(NULL));
   createAsteroids();
   createStars();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   glPolygonMode(GL_FRONT_AND_BACK, mode());
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   ilumination();

   glEnable(GL_COLOR_MATERIAL);
}

void removeLastAsteroids()
{
   for (int i = 0; i < asteroids.size();)
   {
      float dx = asteroids[i].x - camX;
      float dy = asteroids[i].y - camY;
      float dz = asteroids[i].z - camZ;
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
   steady_clock::time_point now = steady_clock::now();
   duration<float> elapsed = now - lastAsteroidTime;

   if (elapsed.count() >= 0.2f)
   {
      lastAsteroidTime = now;
      Sphere asteroid;
      float newDistance = 100.0f;

      float randX = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
      float randY = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
      float randZ = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;

      asteroid.x = camX + dirX * newDistance + randX;
      asteroid.y = camY + dirY * newDistance + randY;
      asteroid.z = camZ + dirZ * newDistance + randZ;
      asteroids.push_back(asteroid);
   }
}

void updateStars()
{
   for (Sphere &star : stars)
   {
      float dx = star.x - camX;
      float dy = star.y - camY;
      float dz = star.z - camZ;

      float distance = sqrt(dx * dx + dy * dy + dz * dz);

      if (distance < 90.0f)
      {
         float newDistance = 100.0f;

         float randX = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
         float randY = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
         float randZ = ((float)rand() / RAND_MAX - 0.5f) * 30.0f;

         star.x = camX + dirX * newDistance + randX;
         star.y = camY + dirY * newDistance + randY;
         star.z = camZ + dirZ * newDistance + randZ;
      }
   }
}

void moveCam(float deltaTime)
{
   float direction = movingBackward ? -1.0f : 1.0f;
   camX += dirX * camSpeed * deltaTime * direction;
   camY += dirY * camSpeed * deltaTime * direction;
   camZ += dirZ * camSpeed * deltaTime * direction;
}

void updateVectorDir(float deltaTime)
{
   angleY += rxSpeed * deltaTime;
   angleX += rzSpeed * deltaTime;

   if (angleX > 89.0f)
      angleX = 89.0f;
   if (angleX < -89.0f)
      angleX = -89.0f;

   float radY = angleY * M_PI / 180.0f;
   float radX = angleX * M_PI / 180.0f;

   dirX = cos(radX) * sin(radY);
   dirY = sin(radX);
   dirZ = cos(radX) * cos(radY);
}

float fpsTimer = 0.0f;

void fpsControl(float deltaTime)
{
   fpsTimer += deltaTime;
   frames++;

   if (fpsTimer >= 1.0f)
   {
      currentFPS = frames;
      frames = 0;
      fpsTimer = 0.0f;
   }
}

void update(int value)
{
   auto now = high_resolution_clock::now();
   float deltaTime = duration<float>(now - lastFrameTime).count();
   lastFrameTime = now;

   updateVectorDir(deltaTime);
   moveCam(deltaTime);
   fpsControl(deltaTime);

   updateStars();
   updateAsteroids();
   removeLastAsteroids();

   glutPostRedisplay();

   glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
   key = tolower(key);
   switch (key)
   {
   case 27:
      exit(0);
      break;

   case 'a':
      rxSpeed = +ROTATION_SPEED;
      break;
   case 'd':
      rxSpeed = -ROTATION_SPEED;
      break;
   case 'w':
      rzSpeed = ROTATION_SPEED;
      break;
   case 's':
      rzSpeed = -ROTATION_SPEED;
      break;
   case 'x':
      movingBackward = true;
      break;
   case 'p':
      polygonMode = !polygonMode;
      glPolygonMode(GL_FRONT_AND_BACK, mode());
      break;
   }
}

void keyboardUp(unsigned char key, int x, int y)
{
   key = tolower(key);
   switch (key)
   {
   case 'a':
   case 'd':
      rxSpeed = 0.0f;
      break;
   case 'w':
   case 's':
      rzSpeed = 0.0f;
      break;
   case 'x':
      movingBackward = false;
      break;
   }
}

int main()
{
   int argc = 0;

   glutInit(&argc, NULL);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowPosition(0, 0);
   glutInitWindowSize(SCREEN_X, SCREEN_Y);
   glutCreateWindow("Leandro Galbarino");
   onInit();

   glutDisplayFunc(render);

   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyboardUp);

   glutTimerFunc(0, update, 0);
   glutIdleFunc(render);
   glutMainLoop();
   return 0;
}
