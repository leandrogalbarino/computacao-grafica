#ifndef __MENU_H__
#define __MENU_H__

#include "gl_canvas2d.h"
#include "botao.h"
#include "storage.h"

#include <cmath> // Necessário para sqrt() e pow()
#include "LayerManager.h"
#include <string.h>
// #define NUM_BUTTONS 10

typedef struct
{
  int x;
  int y;
  int _x;
  int _y;
} Coordinates;

typedef struct
{
  float r;
  float g;
  float b;
} Color;

std::string img[] = {
    "t1/images/img3.bmp",
    "t1/images/img1.bmp",
    "t1/images/img2.bmp"};

class Menu
{
protected:
  LayerManager *layerManager;
  int operation;
  int posX;
  int posY;
  int posX2;
  int posY2;
  Botao **buttons;
  int numButtons;
  Coordinates coord;
  Color color;

public:
  Menu(int posX, int posY, int posX2, int posY2, int numButtons)
  {
    if (numButtons < 3)
    {
      numButtons = 3;
    }

    this->posX = posX;
    this->posY = posY;
    this->posX2 = posX2;
    this->posY2 = posY2;
    this->numButtons = numButtons;
    layerManager = nullptr;

    color.r = color.g = color.b = 0;
    coord._x = coord._y = coord.x = coord.y = 0;
    operation = -1;

    buttons = new Botao *[numButtons];

    int baseX = posX + 10;
    int baseY = posY + 10;
    int buttonSize = 30;
    int spacing = 10;

    for (int i = 0; i < numButtons; i++)
    {
      int bx = (posX2 - posX > posY2 - posY) ? baseX + i * (buttonSize + spacing) : baseX;
      int by = (posX2 - posX > posY2 - posY) ? baseY : baseY + i * (buttonSize + spacing);
      buttons[i] = new Botao(bx, by, 30, 30);
    }
  }

  void setColor(int r, int g, int b)
  {
    color.r = r / 255;
    color.g = g / 255;
    color.b = b / 255;
  }

  void renderButtons()
  {
    for (int i = 0; i < numButtons; i++)
    {
      buttons[i]->Render();
    }
  }

  bool collisionButtons(int x, int y)
  {
    for (int index = 0; index < numButtons; index++)
    {
      if (buttons[index]->Colidiu(x, y))
      {
        operation = index;
        return true;
      }
    }
    return false;
  }

  void render()
  {
    CV::color(color.r, color.g, color.b);
    CV::rectFill(posX, posY, posX2, posY2); // Criar menu;
    renderButtons();
    if (layerManager)
    {
      layerManager->drawLayers();
    }
  }

  void handleClick(int x, int y, int _x, int _y)
  {
    setCoord(x, y, _x, _y);
    functions();
  }

  void setCoord(int x, int y, int _x, int _y)
  {
    coord.x = x;
    coord.y = y;
    coord._x = _x;
    coord._y = _y;
  }

  virtual void functions() {}
};

class MenuFunctions : public Menu
{
public:
  MenuFunctions(int posX, int posY, int posX2, int posY2, int numButtons)
      : Menu(posX, posY, posX2, posY2, numButtons) {}

  void setLayerManager(LayerManager *lm)
  {
    layerManager = lm;
  }

  void notOperating()
  {
    operation = -1;
  }

  void functions() override
  {
    if (!layerManager || layerManager->getActiveLayer() == -1)
    {
      std::cout << "\nNenhuma camada selecionada!";
      return;
    }

    float radius;
    switch (operation)
    {
    case -1:
      break;
    case 0:
      layerManager->addElements()->addRet(coord.x, coord.y, coord._x, coord._y, color.r, color.g, color.b);
      break;
    case 1:
      radius = sqrt(pow(coord._x - coord.x, 2) + pow(coord._y - coord.y, 2)) / 2;
      layerManager->addElements()->addCircle(coord.x, coord.y, radius, color.r, color.g, color.b);
      break;
    case 2:
      //Retangulo branco para servir como apagador!
      layerManager->addElements()->addRet(coord.x, coord.y, coord._x, coord._y,  1.0, 1.0, 1.0);
      break;
    default:
      break;
    }
  }
};

class MenuLayer : public Menu
{
public:
  MenuLayer(int posX, int posY, int posX2, int posY2, int numButtons) : Menu(posX, posY, posX2, posY2, numButtons)
  {
    layerManager = new LayerManager();
  }

  LayerManager *getLayerManager()
  {
    return layerManager;
  }

  void functions() override
  {

    if (operation == -1)
    {
      return;
    }
    if (operation == 0 || operation == 1 || operation == 2)
    {
      layerManager->addLayer(img[operation].c_str(), 300, 300);
      return;
    }
    if (operation == 3 || operation == 4 || operation == 5)
    {
      layerManager->toggleLayerVisibility(operation - 3);
      operation = -1;
    }
  }
};
#endif
