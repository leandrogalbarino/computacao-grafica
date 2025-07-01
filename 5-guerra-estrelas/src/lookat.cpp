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

#define SCREEN_X 1280
#define SCREEN_Y 720
#define NUM_STARS 500

GLuint texID;

#pragma pack(push, 1)
typedef struct
{
   unsigned short type;
   unsigned int size;
   unsigned short reserved1, reserved2;
   unsigned int offset;
} BMPHeader;

typedef struct
{
   unsigned int size;
   int width, height;
   unsigned short planes;
   unsigned short bitCount;
   unsigned int compression;
   unsigned int sizeImage;
   int xPelsPerMeter, yPelsPerMeter;
   unsigned int clrUsed, clrImportant;
} BMPInfoHeader;
#pragma pack(pop)

// Função para carregar BMP (24 bits, sem compressão)
unsigned char *LoadBMP(const char *filename, int *width, int *height)
{
   FILE *f = fopen(filename, "rb");
   if (!f)
      return NULL;

   BMPHeader header;
   fread(&header, sizeof(BMPHeader), 1, f);
   if (header.type != 0x4D42)
   { // "BM"
      fclose(f);
      return NULL;
   }

   BMPInfoHeader info;
   fread(&info, sizeof(BMPInfoHeader), 1, f);

   if (info.bitCount != 24 || info.compression != 0)
   {
      fclose(f);
      return NULL; // só 24 bits sem compressão
   }

   fseek(f, header.offset, SEEK_SET);

   int size = info.width * info.height * 3;
   unsigned char *data = (unsigned char *)malloc(size);

   fread(data, size, 1, f);
   fclose(f);

   // BMP armazena em BGR, inverter para RGB
   for (int i = 0; i < size; i += 3)
   {
      unsigned char tmp = data[i];
      data[i] = data[i + 2];
      data[i + 2] = tmp;
   }

   *width = info.width;
   *height = info.height;

   return data;
}

const int MAX_ASTEROIDS = 200;

int polygonMode = 0;

float rx = 0, rz = 0; // parametros de rotacao do objeto.

float abertura = 20.0;
float znear = 1;
float zfar = 100;
float aspect = 1;
float camX = 0.0f, camY = 0.0f, camZ = 15.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = 1.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
float rxSpeed = 0.0f;
float rzSpeed = 0.0f;
float camSpeed = 0.1f;
float angleY = 0.0f; // Rotação em torno do eixo Y (horizontal)
float angleX = 0.0f; // Rotação em torno do eixo X (vertical)
int frameCount = 0;

struct Sphere
{
   float x, y, z;
};

std::vector<Sphere> spheres;
std::vector<Sphere> stars;
const int NUM_SPHERES = 500;
const float MAP_SIZE = 20.0f;     // Limite do espaço (ex: -10 a +10 em x, y, z)
const float MAP_SIZE_STAR = 2.0f; // Limite do espaço (ex: -10 a +10 em x, y, z)

void drawStars()
{
   glPointSize(2.0f); // Tamanho bem pequeno
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
   GLUquadric *quad = gluNewQuadric();
   gluQuadricTexture(quad, GL_TRUE);

   glBindTexture(GL_TEXTURE_2D, texID);
   glEnable(GL_TEXTURE_2D);

   for (const Sphere &s : spheres)
   {
      glPushMatrix();
      glTranslatef(s.x, s.y, s.z);
      gluSphere(quad, 0.5, 16, 16);
      glPopMatrix();
   }

   glDisable(GL_TEXTURE_2D);
   gluDeleteQuadric(quad);
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

void createAsteroids()
{
   for (int i = 0; i < NUM_SPHERES; i++)
   {
      Sphere s;

      // Gera uma distância aleatória entre 20 e 500
      float distancia = 20.0f + static_cast<float>(rand()) / RAND_MAX * (500.0f - 20.0f);

      // Gera um vetor de direção aleatória (ponto sobre uma esfera)
      float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI; // ângulo azimutal
      float phi = static_cast<float>(rand()) / RAND_MAX * M_PI;          // ângulo polar

      // Coordenadas esféricas para cartesianas
      float dx = sin(phi) * cos(theta);
      float dy = sin(phi) * sin(theta);
      float dz = cos(phi);

      // Posição final do asteroide ao redor da câmera
      s.x = camX + dx * distancia;
      s.y = camY + dy * distancia;
      s.z = camZ + dz * distancia;

      spheres.push_back(s);
   }
}

// OK
void createStars()
{
   stars.clear(); // limpa as antigas

   for (int i = 0; i < NUM_STARS; i++)
   {
      Sphere s;

      float distanciaFrente = 30.0f + static_cast<float>(rand()) / RAND_MAX * (300.0f - 30.0f);
      float variacaoLateral = 50.0f;
      float variacaoVertical = 50.0f;

      float baseX = camX + dirX * distanciaFrente;
      float baseY = camY + dirY * distanciaFrente;
      float baseZ = camZ + dirZ * distanciaFrente;

      float offsetX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoLateral;
      float offsetY = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoVertical;
      float offsetZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f * variacaoLateral;

      s.x = baseX + offsetX;
      s.y = baseY + offsetY;
      s.z = baseZ + offsetZ;

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
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
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

   // Carrega textura BMP e cria textura OpenGL
   int width, height;
   unsigned char *image = LoadBMP("./5-guerra-estrelas/src/textura.bmp", &width, &height);
   if (image)
   {
      glGenTextures(1, &texID);
      glBindTexture(GL_TEXTURE_2D, texID);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, image);

      free(image);

      glEnable(GL_TEXTURE_2D);
   }
   else
   {
      printf("Falha ao carregar textura.\n");
   }
}

// OK
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

void moveCam()
{
   camX += dirX * camSpeed;
   camY += dirY * camSpeed;
   camZ += dirZ * camSpeed;
}

void updateVectorDir()
{
   angleY += rxSpeed * 0.016f;
   angleX += rzSpeed * 0.016f;

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

void update(int value)
{
   updateVectorDir();
   moveCam();
   updateStars();

   glutPostRedisplay();
   glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
   key = tolower(key);
   switch (key)
   {
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
      polygonMode = !polygonMode;
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

   glutTimerFunc(0, update, 0); // <== inicia loop de atualização
   glutIdleFunc(render);
   glutMainLoop();
   return 0;
}
