#ifndef __MENU_H__
#define __MENU_H__

#include "gl_canvas2d.h"
#include "botao.h"
#include "storage.h"

#include <cmath> // Necessário para sqrt() e pow()
#include "LayerManager.h"
#include <string.h>
#include "Checkbox.h"
#include "Slider.h"
#include "Coordinates.h"

#define SLIDER_START_X 1285
#define SLIDER_END_X 1520
#define SLIDER_START_Y 100
#define SLIDER_SPACING_Y 30

#define COLOR_PREVIEW_X1 1380
#define COLOR_PREVIEW_Y1 40
#define COLOR_PREVIEW_X2 1420
#define COLOR_PREVIEW_Y2 80

#define SLIDER_INDICATOR_X1 1258
#define SLIDER_INDICATOR_X2 1278
#define SLIDER_INDICATOR_Y_START 90

#define BUTTON_SIZE 30
#define BUTTON_SPACING 10

#define DRAW_AREA_X1 100
#define DRAW_AREA_Y1 50
#define DRAW_AREA_X2 1100
#define DRAW_AREA_Y2 650

#define MENU_COLOR_R 0.10
#define MENU_COLOR_G 0.1
#define MENU_COLOR_B 0.1

#define SLIDER_LENGHT 3
enum RGB
{
  R,
  G,
  B
};

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
  int disableLayer;
  Botao **buttons;
  int numButtons;
  Slider **slider;

  Coordinates coords;
  Coordinates mouseCoords;

  Color color;

private:
  void createSlider()
  {
    slider = new Slider *[SLIDER_LENGHT];
    for (int i = 0; i < SLIDER_LENGHT; i++)
    {
      Coordinates coordSlider(SLIDER_START_X, SLIDER_START_Y + SLIDER_SPACING_Y * i,
                              SLIDER_END_X, SLIDER_START_Y + SLIDER_SPACING_Y * i);
      slider[i] = new Slider(coordSlider);
    }
  }

protected:
  virtual void createButtons()
  {
    buttons = new Botao *[numButtons];
    int baseX = coords.x1 + 10;
    int baseY = coords.y1 + 10;
    int buttonSize = 30;
    int spacing = 10;

    for (int i = 0; i < numButtons; i++)
    {
      int bx = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseX + i * (buttonSize + spacing) : baseX;
      int by = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseY : baseY + i * (buttonSize + spacing);
      buttons[i] = new Botao(bx, by, BUTTON_SIZE, BUTTON_SIZE);
    }
  }

  virtual void renderButtons()
  {
    for (int i = 0; i < numButtons; i++)
    {
      buttons[i]->Render();
    }
  }

public:
  Menu(Coordinates coords, int numButtons)
  {
    this->coords = coords;
    this->numButtons = (numButtons >= 3) ? numButtons : 3;

    layerManager = nullptr;
    mouseCoords.x1 = mouseCoords.y1 = mouseCoords.x2 = mouseCoords.y2 = 0;
    operation = -1;
    disableLayer = -1;
  }

  virtual void init()
  {
    createButtons();
    createSlider();
    setColor();
  }

  void setColor()
  {
    color.r = slider[R]->value;
    color.g = slider[G]->value;
    color.b = slider[B]->value;
  }

  virtual bool collisionButtons(int x, int y)
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

  void handleClick(int x1, int y1, int x2, int y2)
  {
    setCoord(x1, y1, x2, y2);
    for (int i = 0; i < SLIDER_LENGHT; i++)
    {
      slider[i]->render();
      if (slider[i]->isHovering(x2, y2))
      {
        slider[i]->setPointer(x2);
      }
    }
    functions();
  }

  void setCoord(int x1, int y1, int x2, int y2)
  {
    mouseCoords.x1 = x1;
    mouseCoords.y1 = y1;
    mouseCoords.x2 = x2;
    mouseCoords.y2 = y2;
  }

  virtual void functions()
  {
    setColor();
  }

  void renderSliderRGB()
  {
    CV::color(color.r, color.g, color.b);
    CV::rectFill(COLOR_PREVIEW_X1, COLOR_PREVIEW_Y1, COLOR_PREVIEW_X2, COLOR_PREVIEW_Y2);
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
      CV::rectFill(SLIDER_INDICATOR_X1, SLIDER_INDICATOR_Y_START + SLIDER_SPACING_Y * i,
                   SLIDER_INDICATOR_X2, SLIDER_INDICATOR_Y_START + 20 + SLIDER_SPACING_Y * i);
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
    CV::color(MENU_COLOR_R, MENU_COLOR_G, MENU_COLOR_B);      // Cor preta;
    CV::rectFill(coords.x1, coords.y1, coords.x2, coords.y2); // Criar menu;
    renderButtons();
  }
};

class MenuFunctions : public Menu
{
public:
  MenuFunctions(Coordinates coords, int numButtons) : Menu(coords, numButtons) {}

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
      layerManager->layerActive()->addRect(mouseCoords, color.r, color.g, color.b);
      break;
    case 1:
      radius = sqrt(pow(mouseCoords.x2 - mouseCoords.x1, 2) + pow(mouseCoords.y2 - mouseCoords.y1, 2)) / 2;
      layerManager->layerActive()->addCircle(mouseCoords.x1, mouseCoords.y1, radius, color.r, color.g, color.b);
      break;
    case 2:
      layerManager->layerActive()->addLine(mouseCoords, color.r, color.g, color.b);
      break;
    case 3:
      layerManager->layerActive()->addPoint(mouseCoords.x1, mouseCoords.y1, color.r, color.g, color.b);
      break;
    case 4:
      // Ponto maior
      radius = 10.0;
      layerManager->layerActive()->addCircle(mouseCoords.x1, mouseCoords.y1, radius, color.r, color.g, color.b);
      break;
    case 5:
      layerManager->layerActive()->removeElement(mouseCoords.x2, mouseCoords.y2);
      break;
    default:
      break;
    }
  }
};

class MenuLayer : public Menu
{
public:
  CheckBox **checkBox;

  MenuLayer(Coordinates coords, int numButtons) : Menu(coords, numButtons)
  {
    this->coords = coords;
    Coordinates drawArea(DRAW_AREA_X1, DRAW_AREA_Y1, DRAW_AREA_X2, DRAW_AREA_Y2);
    layerManager = new LayerManager(drawArea);
  }

  // Criar botões e checkbox
  void createButtons() override
  {
    buttons = new Botao *[numButtons];
    checkBox = new CheckBox *[numButtons];

    int baseX = coords.x1 + 10;
    int baseY = coords.y1 + 10;
    int buttonSize = 30;
    int spacing = 10;
    int checkboxSize = 15;

    for (int i = 0; i < numButtons; i++)
    {
      int bx = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseX + i * (buttonSize + spacing) : baseX;
      int by = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseY : baseY + i * (buttonSize + spacing);

      Coordinates coordsCheckBox(bx - checkboxSize - spacing*2, by + spacing/2, bx - spacing*2, by + spacing/2 + checkboxSize);

      checkBox[i] = new CheckBox(coordsCheckBox);
      buttons[i] = new Botao(bx, by, BUTTON_SIZE, BUTTON_SIZE);
    }
  }

  void renderButtons() override
  {
    for (int i = 0; i < numButtons; i++)
    {
      buttons[i]->Render();
      checkBox[i]->render();
    }
  }

  virtual bool collisionButtons(int x, int y)
  {
    for (int index = 0; index < numButtons; index++)
    {
      if (buttons[index]->Colidiu(x, y))
      {
        operation = index;
        return true;
      }
      if (checkBox[index]->click(x, y))
      {
        disableLayer = index;
        return true;
      }
    }
    return false;
  }

  LayerManager *getLayerManager()
  {
    return layerManager;
  }

  void functions() override
  {

    if (disableLayer != -1)
    {
      layerManager->toggleLayerVisibility(disableLayer);
      disableLayer = -1;
      operation = -1;
      return;
    }

    if (operation != -1 && operation < numButtons)
    {
      layerManager->addLayer(img[operation].c_str(), 50, 0);
      if (!checkBox[layerManager->getActiveLayer()]->isChecked())
      {
        checkBox[layerManager->getActiveLayer()]->setCheck(true);
      }
      return;
    }
  }
};
#endif
