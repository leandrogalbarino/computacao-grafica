
#include <GL/glut.h>

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define SCREEN_X 800
#define SCREEN_Y 600

// Vetores de câmera
float camX = 0.0f, camY = 0.0f, camZ = 10.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

// Timer
int lastTime = 0;
float fps = 0.0f;
bool wireframe = false;

void initLighting() {
    GLfloat light_pos[] = { 0.0f, 10.0f, 10.0f, 1.0f };
    GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

void drawAsteroids() {
    for (int i = 0; i < 10; ++i) {
        glPushMatrix();
        glTranslatef(i * 2.0f - 10, 0.0f, -20.0f - i * 5);
        glutSolidSphere(1.0, 16, 16);
        glPopMatrix();
    }
}

void drawShip() {
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    glutSolidCone(0.5, 1.0, 16, 16);
    glPopMatrix();
}



void onInit()
{
        glClearColor(0.0, 0.0, 0.0, 1.0);

    GLfloat light_pos[] = { 0.0f, 10.0f, 10.0f, 1.0f };
    GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}


void render(void)
{
   int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int delta = currentTime - lastTime;
    fps = 1000.0f / delta;
    lastTime = currentTime;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camX, camY, camZ, camX + dirX, camY + dirY, camZ + dirZ, upX, upY, upZ);

   drawAsteroids();
   drawShip();

   glutSwapBuffers();
}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   //printf("%c", key);
   key = tolower(key);
   if (key == 'd') {
       camX += 0.05f;
       //  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }
    if (key == 'a') {
       camX -= 0.05f;
       //  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }if (key == 's') {
       camY -= 0.05f;
       //  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }if (key == 'w') {
       camY += 0.05f;
       //  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }
    
}

void update(int value) {
    // Movimento contínuo da câmera (pode ajustar depois com entrada real)
    camZ -= 0.05f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}
void reshape(int w, int h)
{
    if (h == 0) h = 1;
    float aspect = (float)w / h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 1.0, 1000.0);
}

void MotionFunc(int x, int y)
{

}

void MouseFunc(int botao, int estado, int x, int y)
{
   //printf("\n%d %d %d %d", botao, estado, x, y);
}

int main ()
{
   int  argc=0;

   glutInit(&argc, NULL);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowPosition (450, 10);
   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutCreateWindow ("Leandro Galbarino");
   glutReshapeFunc(reshape);
   onInit();

   // Callback mais importante. Usada para especificação da função de desenho. Chamada sempre que o contexto da janela precisa ser reexibido. 
   glutDisplayFunc(render);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(0, update, 0);
   //funções para tratamento de teclado ou botão de mouse
   // glutMouseFunc(MouseFunc);
   
   // trata movimento do mouse, enquanto algum botão estiver pressionado.
   // glutMotionFunc(MotionFunc); 
   
//  função chamada sempre que não existem eventoa serem tratados. 
   // glutIdleFunc(render);
   glutMainLoop();
   return 0;
}
