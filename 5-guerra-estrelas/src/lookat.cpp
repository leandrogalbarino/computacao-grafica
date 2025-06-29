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

int polygonMode = 1;

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
struct Sphere
{
   float x, y, z;
};

std::vector<Sphere> spheres;
std::vector<Sphere> stars;
const int NUM_SPHERES = 20;
const float MAP_SIZE = 20.0f;     // Limite do espaço (ex: -10 a +10 em x, y, z)
const float MAP_SIZE_STAR = 2.0f; // Limite do espaço (ex: -10 a +10 em x, y, z)

void onInit()
{
   srand(time(NULL)); // Semente para aleatoriedade

   for (int i = 0; i < NUM_SPHERES; i++)
   {
      Sphere s;

      float distanciaFrente = 10.0f; // distância à frente da câmera
      float variacaoLateral = 5.0f;  // variação lateral (esquerda/direita)
      float variacaoVertical = 3.0f; // variação vertical (cima/baixo)

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

   for (int i = 0; i < 100; i++)
   {
      Sphere s;
      s.x = ((float)rand() / RAND_MAX) * MAP_SIZE - MAP_SIZE / 2.0f;
      s.y = ((float)rand() / RAND_MAX) * MAP_SIZE - MAP_SIZE / 2.0f;
      s.z = ((float)rand() / RAND_MAX) * MAP_SIZE - MAP_SIZE / 2.0f;
      stars.push_back(s);
   }

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glEnable(GL_DEPTH_TEST);
}

void desenharEstrelas()
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

////////////////////////////////////////////////////////////////////////////////////////
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
             upX, upY, upZ); // vetor "para cima"
   desenharEstrelas();

   for (const Sphere &s : spheres)
   {
      glPushMatrix();
      glColor3f(0.7f, 0.7f, 0.7f); // Cor da esfera
      glTranslatef(s.x, s.y, s.z);
      glutSolidSphere(0.5, 16, 16); // Raio 0.5, 16 slices e stacks
      glPopMatrix();
   }

   // glRotatef((GLfloat)rx, 0.0f, 1.0f, 0.0f);
   // glRotatef((GLfloat)rz, 1.0f, 0.0f, 0.0f);

   // todos os objetos estao definidos na origem do sistema global fixo
   // e sao transladados para a posicao destino.
   glColor3f(1, 1, 1);

   // glVertex*(), glColor*(),glIndex*(), glNormal*(), glTextCoord*() e glMaterial*()

   //    Marca o início de uma lista de vértices que descrevem uma primitiva geométrica. O parâmetro mode pode ser uma das 10 primitivas geométricas descritas anteriormente

   //    glVertex3d();
   //    glColor*(), glNormal*(), glFog*(), glLight*(), glMaterial*(),
   // glRotate*(), glTranslate*(), glScale*()
   // bule 3
   glColor3f(0, 0, 1);
   glutSolidTeapot(1.2);

   glutSwapBuffers();

   //  void glPointSize(GLfloat size)
   // void glLineWidth(GLfloat width):
   // void glPolygonMode(GLenum face, GLenum mode);
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

int frameCount = 0;

void updateStars()
{
   for (Sphere &star : stars)
   {
      // Distância da estrela até a câmera
      float dx = star.x - camX;
      float dy = star.y - camY;
      float dz = star.z - camZ;

      float distancia = sqrt(dx * dx + dy * dy + dz * dz);

      // Se estiver muito perto da câmera, reposiciona para longe
      if (distancia < 40.0f)
      {
         float distanciaNova = 100.0f;

         // Reposiciona na frente da câmera com variação aleatória
         star.x = camX + dirX * distanciaNova + ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
         star.y = camY + dirY * distanciaNova + ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
         star.z = camZ + dirZ * distanciaNova + ((float)rand() / RAND_MAX - 0.5f) * 30.0f;
      }
   }
}
void update(int value)
{
   // Atualiza ângulos e câmera (já existente)
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

   // Incrementa contador de frames
   frameCount++;

   // A cada 60 frames (~1 segundo), adiciona uma nova esfera
   if (frameCount >= 20)
   {
      frameCount = 0;
      Sphere s;

      float distanciaFrente = 50.0f; // distância à frente da câmera
      float variacaoLateral = 5.0f;  // variação lateral (esquerda/direita)
      float variacaoVertical = 3.0f; // variação vertical (cima/baixo)

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
   updateStars();

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
   glutMouseFunc(MouseFunc);
   // trata movimento do mouse, enquanto algum botão estiver pressionado.
   glutMotionFunc(MotionFunc);

   glutTimerFunc(0, update, 0); // <== inicia loop de atualização

   //  função chamada sempre que não existem eventoa serem tratados.
   glutIdleFunc(render);

   glutMainLoop();
   return 0;
}
