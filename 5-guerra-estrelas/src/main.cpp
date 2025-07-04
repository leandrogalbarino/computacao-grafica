// Função principal que inicializa a janela, controla o loop da aplicação e gerencia entrada do usuário. A cena 3D inclui câmera com movimentação, asteroides, estrelas e iluminação.

#include <GL/glut.h>

#include <ctype.h>
#include <cstdlib>
#include "Fps.h"
#include "Map.h"

#define SCREEN_X 1280
#define SCREEN_Y 720

Fps fps;
Map map;
Camera cam;

void render(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(cam.oppening, cam.aspect, cam.znear, cam.zfar);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt(cam.coord.x, cam.coord.y, cam.coord.z,
             cam.coord.x + cam.dir.x, cam.coord.y + cam.dir.y, cam.coord.z + cam.dir.z,
             cam.up.x, cam.up.y, cam.up.z);

   map.render();
   glutSwapBuffers();
}

void onInit()
{
   srand(time(NULL));
   map.init(&fps, &cam);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(cam.oppening, cam.aspect, cam.znear, cam.zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   glPolygonMode(GL_FRONT_AND_BACK, map.modeView());
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   map.ilumination();
   glEnable(GL_COLOR_MATERIAL);
}

void update(int value)
{
   float deltaTime = fps.deltaTime();
   fps.control(deltaTime);
   cam.update(deltaTime);
   map.update();
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
      cam.rxSpeed = +ROTATION_SPEED;
      break;
   case 'd':
      cam.rxSpeed = -ROTATION_SPEED;
      break;
   case 'w':
      cam.rzSpeed = ROTATION_SPEED;
      break;
   case 's':
      cam.rzSpeed = -ROTATION_SPEED;
      break;
   case 'x':
      cam.movingBackward = true;
      break;
   case 'p':
      map.alterMode();
      glPolygonMode(GL_FRONT_AND_BACK, map.modeView());
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
      cam.rxSpeed = 0.0f;
      break;
   case 'w':
   case 's':
      cam.rzSpeed = 0.0f;
      break;
   case 'x':
      cam.movingBackward = false;
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
