#ifndef __MENU_H__
#define __MENU_H__

#include "gl_canvas2d.h"
#include "botao.h"
#include "storage.h"

#include <cmath> // Necessário para sqrt() e pow()
#include "LayerManager.h"
#include <string.h>
#include "Slider.h"

#define SLIDER_LENGHT 3
enum
{
  R,
  G,
  B
} rgb;

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
  Slider **slider;
  Coordinates coord;
  Color color;

private:
  void createSlider()
  {
    slider = new Slider *[SLIDER_LENGHT];
    for (int i = 0; i < SLIDER_LENGHT; i++)
    {
      slider[i] = new Slider(1285, 100 + 30 * i, 1520, 100 + 30 * i);
    }
  }
  void createButtons()
  {
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

  void renderButtons()
  {
    for (int i = 0; i < numButtons; i++)
    {
      buttons[i]->Render();
    }
  }

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

    coord._x = coord._y = coord.x = coord.y = 0;
    operation = -1;
    color.r = color.g = color.b = 0;
    createSlider();
    createButtons();
  }

  void setColor()
  {
    color.r = slider[R]->value;
    color.g = slider[G]->value;
    color.b = slider[B]->value;
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

  void renderSliderRGB()
  {
    CV::color(color.r, color.g, color.b);
    CV::rectFill(1380, 40, 1420, 80);
    for (int i = 0; i < SLIDER_LENGHT; i++)
    {
      switch (i)
      {
      case R:
        CV::color(1.0, 0, 0);
        break;
      case G:
        CV::color(0, 1.0, 0);
        break;
      case B:
        CV::color(0, 0, 1.0);
        break;
      }
      CV::rectFill(1258, 90 + 30 * i, 1278, 110 + 30 * i);
      slider[i]->render();
    }
  }
  
  void renderLayers()
  {
    if (layerManager)
    {
      layerManager->drawLayers();
    }
  }

  void render()
  {
    CV::color(0, 0, 0);                     // cor do menu
    CV::rectFill(posX, posY, posX2, posY2); // Criar menu;
    renderButtons();
  }

  void handleClick(int x, int y, int _x, int _y)
  {
    setCoord(x, y, _x, _y);
    for (int i = 0; i < SLIDER_LENGHT; i++)
    {
      slider[i]->render();
      if (slider[i]->isHovering(_x, _y))
      {
        slider[i]->setPointer(x); // atualiza ponteiro e valor baseado na posição do mouse
      }
    }
    functions();
  }

  void setCoord(int x, int y, int _x, int _y)
  {
    coord.x = x;
    coord.y = y;
    coord._x = _x;
    coord._y = _y;
  }

  virtual void functions()
  {
    setColor();
  }
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

    setColor();
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
      layerManager->layerActive()->addRect(coord.x, coord.y, coord._x, coord._y, color.r, color.g, color.b);
      break;
    case 1:
      radius = sqrt(pow(coord._x - coord.x, 2) + pow(coord._y - coord.y, 2)) / 2;
      layerManager->layerActive()->addCircle(coord.x, coord.y, radius, color.r, color.g, color.b);
      break;
    case 2:
      layerManager->layerActive()->addLine(coord.x, coord.y, coord._x, coord._y, color.r, color.g, color.b);
      break;
    case 3:
      layerManager->layerActive()->addPoint(coord.x, coord.y, color.r, color.g, color.b);
      break;
    case 4:
      // Ponto maior
      radius = 10.0;
      layerManager->layerActive()->addCircle(coord.x, coord.y, radius, color.r, color.g, color.b);
      break;
    case 5:
      // Apaga elementos adicionados seja qual for!
      layerManager->layerActive()->removeElement(coord._x, coord._y);
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
    layerManager = new LayerManager(100, 50, 1150, 650);
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
      layerManager->addLayer(img[operation].c_str(), 50, 0);
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
