#ifndef __MENU_H_
#define __MENU_H_

#include "gl_canvas2d.h"
#include "Tank.h"
#include "Barrels.h"
#include "Map.h"
#include "Button.h"
#include "Vector2.h"

#define GAME_RUN 0
#define GAME_OVER 1
#define GAME_WIN 2
#define CARAC 12 // carcteres das mensagens;
#define NEW_GAME 0
#define END_GAME 1
#define CLICK 1

class Menu
{
private:
  std::mt19937 *gen;
  int screenWidth;
  int screenHeight;

public:
  int gameStatus;
  Tank *tank;
  Barrels *barrels;
  Map *map;
  Button bControls[2];

  Menu(int width, int height, std::mt19937 *generator)
  {
    gameStatus = GAME_RUN;
    screenWidth = width;
    screenHeight = height;
    gen = generator;
    createButtons();
    createGame();
  }

  void createButtons()
  {
    int textWidth = std::max(
                        (int)strlen("Novo Jogo!"),
                        (int)strlen("Fechar!")) *
                    CV::getPixelsTextSize();
    Vector2 centroTela(screenWidth / 2, screenHeight / 2);

    int alturaBotao = 50;
    int espacamento = 10;

    for (int i = 0; i < 2; i++)
    {
      float top = (centroTela.y - 30) - ((alturaBotao + espacamento) * i);
      float bottom = top - alturaBotao;

      bControls[i].setPosition(
          centroTela.x - textWidth / 2,
          bottom,
          centroTela.x + textWidth / 2,
          top);
    }
    bControls[NEW_GAME].setMensage("Novo Jogo!");
    bControls[END_GAME].setMensage("Fechar!");
  }

  void createGame()
  {
    tank = new Tank();
    barrels = new Barrels(gen);
    map = new Map(tank, barrels);
    gameStatus = GAME_RUN;
  }

  void displacerGame()
  {
    delete tank;
    delete barrels;
    delete map;
    tank = nullptr;
    map = nullptr;
    barrels = nullptr;
  }

  void newGame()
  {
    displacerGame();
    createGame();
  }

  void game()
  {
    map->render();
    if (tank->shoot)
    {
      map->collideProj(tank->shootVectorNew);
      Barrel *barrel = barrels->collideBarrel(tank->shootVectorOld, tank->shootVectorNew);
      if (barrel)
      {
        tank->setProjectil(false);
        if (barrels->updateBarrelLife(barrel))
          tank->score++;
      }
    }
    map->collideTank();

    if (tank->life <= 0)
      gameStatus = GAME_OVER;
    if (barrels->getBarrels().size() <= 0)
      gameStatus = GAME_WIN;
  }

  void gameFinish()
  {
    char mensageText[50];
    char scoreText[50];
    if (gameStatus == GAME_WIN)
      sprintf(mensageText, "Voce venceu!");
    else
      sprintf(mensageText, "Voce perdeu!");
    sprintf(scoreText, "Pontuacao: %d", tank->score);

    int mensageWidth = strlen(mensageText) * CV::getPixelsTextSize();
    int scoreWidth = strlen(scoreText) * CV::getPixelsTextSize();

    Vector2 centerScreen(screenWidth / 2, screenHeight / 2);
    Vector2 p1Text(centerScreen.x - mensageWidth / 2, centerScreen.y);
    Vector2 p2Text(centerScreen.x - scoreWidth / 2, centerScreen.y - 20);

    CV::color(RED);
    CV::text(p1Text, mensageText);
    CV::color(BLACK);
    CV::text(p2Text, scoreText);
    bControls[NEW_GAME].render();
    bControls[END_GAME].render();
  }

  // EVENTS
  void buttonsEvents(float x, float y, int state)
  {
    if (state == CLICK)
    {

      if (bControls[NEW_GAME].collided(x, y))
      {
        newGame();
      }
      else if (bControls[END_GAME].collided(x, y))
      {
        exit(0);
      }
    }
  }

  void keyTankEvents(int key, bool value)
  {
    if (key == 'a' || key == 'A')
      tank->setTurningLeft(value);
    if (key == 'd' || key == 'D')
      tank->setTurningRight(value);
  }

  void
  mouseTankEvents(float x, float y, int state)
  {
    tank->setMousePositon(x, y);
    if (state == CLICK)
      tank->setProjectil(true);
  }

  void gameLoop()
  {
    if (gameStatus == GAME_RUN)
      game();
    else
      gameFinish();
  }
};
#endif