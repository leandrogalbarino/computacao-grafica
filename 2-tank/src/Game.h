#ifndef __GAME_H_
#define __GAME_H_

#include "gl_canvas2d.h"
#include "Tank.h"
#include "Barrels.h"
#include "Map.h"
#include "Button.h"
#include "Vector2.h"

#define GAME_ALTER -1
#define GAME_RUN 0

#define GAME_OVER 1
#define GAME_WIN 2
#define GAME_MENU 3

#define NEW_GAME 0
#define ALTER_MAP_POINTS 1
#define END_GAME 2
#define CLICK 1
#define N_BUTTONS_MENU 3
#define BUTTON_ALTER_MAP 3

// #define TYPE_ALTER
// #define TYPE_NORMAL

class Game
{
private:
  std::mt19937 *gen;
  int screenWidth;
  int screenHeight;

public:
  int status;
  Tank *tank;
  Barrels *barrels;
  Map *map;
  Button bControls[4];
  std::vector<Vector2> points;
  int type;

  Game(int width, int height, std::mt19937 *generator)
  {
    screenWidth = width;
    screenHeight = height;
    tank = nullptr;
    barrels = nullptr;
    map = nullptr;
    gen = generator;
    status = GAME_MENU;
    type = NEW_GAME;
    createButtons();
  }

  void createButtonAlter()
  {
    const int padding = 10;
    int textWidth = (strlen("Criar Pista!") + padding) * CV::getPixelsTextSize();
    Vector2 centroTela(screenWidth / 2, screenHeight / 2);
    int alturaBotao = 50;
    int espacamento = 10;
    float top = (centroTela.y - 30) - ((alturaBotao + espacamento));
    float bottom = top - alturaBotao;

    bControls[BUTTON_ALTER_MAP].setPosition(
        centroTela.x - textWidth / 2,
        bottom,
        centroTela.x + textWidth / 2,
        top);

    bControls[BUTTON_ALTER_MAP].setMensage("Criar Pista!");
  }

  void createButtons()
  {
    int textWidth = std::max(
                        (int)strlen("Novo Jogo!"), (int)strlen("Editar Pista!")) *
                    CV::getPixelsTextSize();
    Vector2 centroTela(screenWidth / 2, screenHeight / 2);

    int alturaBotao = 50;
    int espacamento = 10;

    for (int i = 0; i < N_BUTTONS_MENU; i++)
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
    bControls[ALTER_MAP_POINTS].setMensage("Editar Pista!");
    createButtonAlter();
  }

  void createGame()
  {
    tank = new Tank();
    barrels = new Barrels();
    map = new Map(tank, barrels, gen);
    map->init(type, points);
    status = GAME_RUN;
  }

  void displacerGame()
  {
    points.clear();
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
    type = NEW_GAME;
    createGame();
  }

  void createAlterMap()
  {
    displacerGame();
    type = ALTER_MAP_POINTS;
    status = GAME_ALTER;
  }

  void createMapAlter(float x, float y, int state)
  {
    if (state == CLICK)
    {
      if (bControls[BUTTON_ALTER_MAP].collided(x, y))
      {
        createGame();
        return;
      }
      points.push_back(Vector2(x, y));
    }
  }

  void tankShootCollideBarrel()
  {
    if (tank->shoot)
    {
      Barrel *barrel = barrels->collideBarrel(tank->shootVectorOld, tank->shootVectorNew);
      if (barrel)
      {
        tank->setProjectil(false);
        if (barrels->updateBarrelLife(barrel))
          tank->score++;
      }
    }
  }

  void tankCollideBarrel()
  {
    Vector2 tankCoord0;
    Vector2 tankCoordsOld;

    for (int i = 0; i <= 4; i++)
    {
      float tankX = tank->tankRect[i].x + tank->origem.x;
      float tankY = tank->tankRect[i].y + tank->origem.y;
      Vector2 tankCoords = Vector2(tankX, tankY);
      if (i == 0)
      {
        tankCoord0 = tankCoords;
        tankCoordsOld = tankCoords;
        continue;
      }

      Barrel *barrel = barrels->collideBarrel(tankCoordsOld, i != 4 ? tankCoords : tankCoord0);
      clock_t now = clock();
      double elapsedMs = (double)(now - tank->lastCollision) * 1000.0 / CLOCKS_PER_SEC;

      if (barrel && elapsedMs > COLLISION_COOLDOWN_MS)
      {
        barrels->updateBarrelLife(barrel);
        tank->UpdateCollision(BARREL_DANO_TANK, now);
        break;
      }
      tankCoordsOld = tankCoords;
    }
  }

  void gameRun()
  {
    map->render();
    tankShootCollideBarrel();
    tankCollideBarrel();
    map->collideProj(tank->shootVectorNew);
    map->collideTank();

    if (tank->life <= 0)
      status = GAME_OVER;
    if (barrels->getBarrels().size() <= 0)
      status = GAME_WIN;
  }

  void gameAlter()
  {
    bControls[BUTTON_ALTER_MAP].render();
    CV::color(RED);
    for (const Vector2 &point : points)
      CV::circleFill(point, 7, 20);
  }

  void gameMenu()
  {
    char mensageText[50];
    char scoreText[50];
    int scoreWidth, mensageWidth;
    Vector2 centerScreen(screenWidth / 2, screenHeight / 2);

    if (status == GAME_WIN)
      sprintf(mensageText, "Voce venceu!");
    else if (status == GAME_OVER)
      sprintf(mensageText, "Voce perdeu!");
    else
      sprintf(mensageText, "GTA 2D");

    CV::color(RED);
    mensageWidth = strlen(mensageText) * CV::getPixelsTextSize();
    Vector2 p1Text(centerScreen.x - mensageWidth / 2, centerScreen.y);
    CV::text(p1Text, mensageText);

    if (status != GAME_MENU)
    {
      CV::color(BLACK);
      sprintf(scoreText, "Pontuacao: %d", tank->score);
      scoreWidth = strlen(scoreText) * CV::getPixelsTextSize();
      Vector2 p2Text(centerScreen.x - scoreWidth / 2, centerScreen.y - 20);
      CV::text(p2Text, scoreText);
    }

    for (int i = 0; i < N_BUTTONS_MENU; i++)
      bControls[i].render();
  }

  // EVENTS
  void buttonsEvents(float x, float y, int state)
  {
    if (state == CLICK)
    {
      if (bControls[NEW_GAME].collided(x, y))
      {
        type = NEW_GAME;
        newGame();
      }
      else if (bControls[ALTER_MAP_POINTS].collided(x, y))
      {
        type = ALTER_MAP_POINTS;
        createAlterMap();
      }
      else if (bControls[END_GAME].collided(x, y))
        exit(0);
    }
  }

  void keyTankEvents(int key, bool value)
  {
    if (key == 'a' || key == 'A')
      tank->setTurningLeft(value);
    if (key == 'd' || key == 'D')
      tank->setTurningRight(value);
  }

  void mouseTankEvents(float x, float y, int state)
  {
    tank->setMousePositon(x, y);
    if (state == CLICK)
      tank->setProjectil(true);
  }

  void loop()
  {
    if (status == GAME_RUN)
      gameRun();
    else if (status == GAME_ALTER)
      gameAlter();
    else
      gameMenu();
  }
};
#endif