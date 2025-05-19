#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.
#include "gl_canvas2d.h"

#include <random>
#include <ctime>
#include "Game.h"

int screenWidth = 1280, screenHeight = 720;
std::random_device rd;
std::mt19937 gen(static_cast<unsigned>(time(0)));
Game *game;

void render()
{
   game->loop();
   Sleep(10);
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   if (game->status == GAME_RUN)
      game->keyTankEvents(key, true);
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   if (game->status == GAME_RUN)
      game->keyTankEvents(key, false);
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   if (game->status == GAME_RUN)
      game->mouseTankEvents(x, y, state);
   else
      game->buttonsEvents(x, y, state);
}

void init()
{
   game = new Game(screenWidth, screenHeight, &gen);
}

int main(void)
{
   init();
   CV::init(&screenWidth, &screenHeight, "Leandro Galbarino - GTA 2D");
   CV::run();
}
