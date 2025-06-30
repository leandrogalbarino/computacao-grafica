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

#define SCREEN_X 500
#define SCREEN_Y 500
const int MAX_ASTEROIDS = 200;

int polygonMode = 0;

float rx = 0, rz = 0; // parametros de rotacao do objeto.

float abertura = 20.0;
float znear = 1;
float zfar = 100;
float aspect = 1;
float camX = 0.0f, camY = 0.0f, camZ = 15.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
float rxSpeed = 0.0f;
float rzSpeed = 0.0f;
float camSpeed = 0.05f;
float angleY = 0.0f; // Rotação em torno do eixo Y (horizontal)
float angleX = 0.0f; // Rotação em torno do eixo X (vertical)
int frameCount = 0;

struct Sphere
{
   float x, y, z;
};

std::vector<Sphere> spheres;
std::vector<Sphere> stars;
const int NUM_SPHERES = 20;
const float MAP_SIZE = 20.0f;     // Limite do espaço (ex: -10 a +10 em x, y, z)
const float MAP_SIZE_STAR = 2.0f; // Limite do espaço (ex: -10 a +10 em x, y, z)

void drawStars()
{
   glPointSize(1.0f); // Tamanho bem pequeno
   glBegin(GL_POINTS);
   glColor3f(1.0f, 1.0f, 1.0f); // Branco brilhante
   for (const Sphere &s : stars)
   {
      glVertex3f(s.x, s.y, s.z);
   }
   glEnd();
}

void drawAsteroids()
{
   for (const Sphere &s : spheres)
   {
      glPushMatrix();
      glColor3f(0.5, 0.5, 0.5); // Cor da esfera
      glTranslatef(s.x, s.y, s.z);
      glutSolidSphere(0.5, 16, 16); // Raio 0.5, 16 slices e stacks
      glPopMatrix();
   }
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

   // Atualiza a posição da luz com base no mundo (não na câmera)
   GLfloat light_pos[] = {5.0f, 0.0f, 5.0f, 1.0f};
   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

   drawStars();
   drawAsteroids();

   glutSwapBuffers();
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
      rxSpeed = -5.0f;
      break;
   case 'd':
      rxSpeed = +5.0f;
      break;
   case 'w':
      rzSpeed = 5.0f;
      break;
   case 's':
      rzSpeed = -5.0f;
      break;
   case 'p':
      polygonMode = (polygonMode == 1) ? 0 : 1;
      glPolygonMode(GL_FRONT_AND_BACK, polygonMode ? GL_FILL : GL_LINE);
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
   }
}

void createAsteroids()
{
   for (int i = 0; i < NUM_SPHERES; i++)
   {
      Sphere s;

      float distanciaFrente = 20.0f + static_cast<float>(rand()) / RAND_MAX * (50.0f - 20.0f);
      float variacaoLateral = 7.0f;  // variação lateral (esquerda/direita)
      float variacaoVertical = 7.0f; // variação vertical (cima/baixo)

      // Gera um ponto na frente da câmera
      float baseX = camX + dirX * distanciaFrente;
      float baseY = camY + dirY * distanciaFrente;
      float baseZ = camZ + dirZ * distanciaFrente;

      // Variação aleatória ao redor da direção
      float offsetX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoLateral;
      float offsetY = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoVertical;
      float offsetZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoLateral;

      // Gera posição final
      s.x = baseX + offsetX;
      s.y = baseY + offsetY;
      s.z = baseZ + offsetZ;
      spheres.push_back(s);
   }
}

void createStars()
{
   for (int i = 0; i < 100; i++)
   {
      Sphere s;
      s.x = ((float)rand() / RAND_MAX) * MAP_SIZE - MAP_SIZE / 2.0f;
      s.y = ((float)rand() / RAND_MAX) * MAP_SIZE - MAP_SIZE / 2.0f;
      s.z = ((float)rand() / RAND_MAX) * MAP_SIZE - MAP_SIZE / 2.0f;
      stars.push_back(s);
   }
}

void onInit()
{
   srand(time(NULL));
   createAsteroids();
   createStars();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, 800, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   GLfloat light_pos[] = {5.0f, 0.0f, 5.0f, 1.0f}; // luz lateral
   GLfloat light_amb[] = {0.05f, 0.05f, 0.05f, 1.0f};
   GLfloat light_dif[] = {1.0f, 1.0f, 1.0f, 1.0f}; // luz bem forte para um lado
   GLfloat light_spe[] = {1.0f, 1.0f, 1.0f, 1.0f};

   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);

   glEnable(GL_COLOR_MATERIAL);
}

void updateStars()
{
   for (Sphere &star : stars)
   {
      float dx = star.x - camX;
      float dy = star.y - camY;
      float dz = star.z - camZ;

      float distancia = sqrt(dx * dx + dy * dy + dz * dz);

      // Se estiver muito perto da câmera, reposiciona para longew
      if (distancia < 90.0f)
      {
         float distanciaNova = 100.0f;

         // Reposiciona na frente da câmera com variação aleatória
         star.x = camX + dirX * distanciaNova + ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
         star.y = camY + dirY * distanciaNova + ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
         star.z = camZ + dirZ * distanciaNova + ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
      }
   }
}

void updateAsteroids()
{

   frameCount++;
   if (spheres.size() < MAX_ASTEROIDS && frameCount >= 20)
   {
      frameCount = 0;
      Sphere s;

      float distanciaFrente = 60.0f;  // distância à frente da câmera
      float variacaoLateral = 8.0f;   // variação lateral (esquerda/direita)
      float variacaoVertical = 15.0f; // variação vertical (cima/baixo)

      // Gera um ponto na frente da câmera
      float baseX = camX + dirX * distanciaFrente;
      float baseY = camY + dirY * distanciaFrente;
      float baseZ = camZ + dirZ * distanciaFrente;

      // Variação aleatória ao redor da direção
      float offsetX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoLateral;
      float offsetY = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoVertical;
      float offsetZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoLateral;

      // Gera posição final
      s.x = baseX + offsetX;
      s.y = baseY + offsetY;
      s.z = baseZ + offsetZ;
      spheres.push_back(s);
   }
}

void cleanOldAsteroids()
{
   std::vector<Sphere> novos;
   for (Sphere &s : spheres)
   {
      // Verifica se o asteroide ainda está à frente da câmera
      float dx = s.x - camX;
      float dy = s.y - camY;
      float dz = s.z - camZ;

      float dist = dx * dirX + dy * dirY + dz * dirZ; // projeção na direção
      if (dist > -10.0f)
      {
         novos.push_back(s); // ainda está visível ou próximo
      }
   }
   spheres = novos;
}

void update(int value)
{
   angleY += rxSpeed * 0.016f;
   angleX += rzSpeed * 0.016f;

   if (angleX > 89.0f)
      angleX = 89.0f;
   if (angleX < -89.0f)
      angleX = -89.0f;

   float radY = angleY * 3.14159265f / 180.0f;
   float radX = angleX * 3.14159265f / 180.0f;

   dirX = cos(radX) * sin(radY);
   dirY = sin(radX);
   dirZ = -cos(radX) * cos(radY);

   camX += dirX * camSpeed;
   camY += dirY * camSpeed;
   camZ += dirZ * camSpeed;
   updateAsteroids();
   updateStars();
   cleanOldAsteroids();
   glutPostRedisplay();
   glutTimerFunc(16, update, 0);
}

int lastMouseX = -1, lastMouseY = -1;
void MotionFunc(int x, int y)
{
   if (lastMouseX >= 0 && lastMouseY >= 0)
   {
      int dx = x - lastMouseX;
      int dy = y - lastMouseY;

      rx += dx * 0.1f; // Ajuste o fator de sensibilidade (0.5f)
      rz += dy * 0.1f;
   }
   lastMouseX = x;
   lastMouseY = y;
}

void MouseFunc(int botao, int estado, int x, int y)
{
   // printf("\n%d %d %d %d", botao, estado, x, y);
}

////////////////////////////////////////////////////////////////////////////////////////
int main()
{
   int argc = 0;

   glutInit(&argc, NULL);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowPosition(450, 10);
   glutInitWindowSize(SCREEN_X, SCREEN_Y);
   glutCreateWindow("Leandro Galbarino");
   onInit();

   // Callback mais importante. Usada para especificação da função de desenho. Chamada sempre que o contexto da janela precisa ser reexibido.
   glutDisplayFunc(render);

   // funções para tratamento de teclado ou botão de mouse
   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyboardUp);
   // glutMouseFunc(MouseFunc);
   // trata movimento do mouse, enquanto algum botão estiver pressionado.
   // glutMotionFunc(MotionFunc);

   glutTimerFunc(0, update, 0); // <== inicia loop de atualização

   //  função chamada sempre que não existem eventoa serem tratados.
   glutIdleFunc(render);

   glutMainLoop();
   return 0;
}
