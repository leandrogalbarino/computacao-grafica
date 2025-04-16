#ifndef __MENU_H__
#define __MENU_H__

#include "gl_canvas2d.h"
#include "button.h"
#include "storage.h"

#include <cmath> // Necessário para sqrt() e pow()
#include "LayerManager.h"
#include <string.h>
#include "Checkbox.h"
#include "Slider.h"
#include "Flip.h"
#include "Coordinates.h"

#define SLIDER_START_X 1320
#define SLIDER_END_X 1520
#define SLIDER_START_Y 100
#define SLIDER_SPACING_Y 30

#define COLOR_PREVIEW_X1 1400
#define COLOR_PREVIEW_Y1 40
#define COLOR_PREVIEW_X2 1440
#define COLOR_PREVIEW_Y2 80

#define SLIDER_INDICATOR_X1 1286
#define SLIDER_INDICATOR_X2 1306
#define SLIDER_INDICATOR_Y_START 90

#define TEXT_OFFSET_RADIUS 8
#define TEXT_OFFSET_BRIGHTNESS 25

#define BUTTON_SIZE 30
#define BUTTON_SPACING 10

#define DRAW_AREA_X1 100
#define DRAW_AREA_Y1 50
#define DRAW_AREA_X2 1100
#define DRAW_AREA_Y2 650

#define MENU_COLOR_R 0.10
#define MENU_COLOR_G 0.1
#define MENU_COLOR_B 0.1

#define MAX_LAYERS 10

#define SLIDER_LENGHT 5
enum RGBRB
{
  R,
  G,
  B,
  RADIUS,
  BRIGHTNESS
};

typedef struct
{
  float r;
  float g;
  float b;
} Color;

std::vector<std::string> img = {
    "t1/images/img3.bmp",
    "t1/images/img1.bmp",
    "t1/images/img1.bmp",
    "t1/images/img1.bmp",
    "t1/images/img1.bmp",
    "t1/images/img1.bmp",
    "t1/images/img1.bmp",
    "t1/images/img1.bmp",
    "t1/images/img2.bmp"};

std::vector<std::string> imgButtons = {
    "t1/images/buttons/add_layer.bmp",
    "t1/images/buttons/swap_down.bmp",
    "t1/images/buttons/swap_up.bmp",

    "t1/images/buttons/add_point.bmp",
    "t1/images/buttons/add_line.bmp",
    "t1/images/buttons/remove_elements.bmp",
    "t1/images/buttons/add_circle.bmp",
    "t1/images/buttons/add_square.bmp",
    "t1/images/buttons/flip_vertical.bmp",
    "t1/images/buttons/flip_horizontal.bmp",
};

class Menu
{
protected:
  LayerManager *layerManager;
  int operation;
  int disableLayer;
  Button **buttons;
  int numButtons;
  Slider **slider;

  Coordinates coords;
  Coordinates mouseCoords;

  Color color;
  float radius;
  float brightness;
  bool attBrightness;

protected:
  void createSliders()
  {
    slider = new Slider *[SLIDER_LENGHT];
    for (int i = 0; i < SLIDER_LENGHT; i++)
    {
      Coordinates coordSlider(SLIDER_START_X, SLIDER_START_Y + SLIDER_SPACING_Y * i,
                              SLIDER_END_X, SLIDER_START_Y + SLIDER_SPACING_Y * i);
      slider[i] = new Slider(coordSlider);
    }
  }

  virtual void createButtons()
  {
    buttons = new Button *[numButtons];
    int baseX = coords.x1 + 10;
    int baseY = coords.y1 + 10;
    int buttonSize = 30;
    int spacing = 10;

    for (int i = 0; i < numButtons; i++)
    {
      int bx = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseX + i * (buttonSize + spacing) : baseX;
      int by = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseY : baseY + i * (buttonSize + spacing);
      buttons[i] = new Button(bx, by, BUTTON_SIZE, BUTTON_SIZE);
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
    this->numButtons = numButtons;
    layerManager = nullptr;
    mouseCoords.x1 = mouseCoords.y1 = mouseCoords.x2 = mouseCoords.y2 = 0;
    operation = -1;
    disableLayer = -1;
    attBrightness = false;
  }

  virtual void init()
  {
    createButtons();
    createSliders();
    setColor();
  }

  void applySliders()
  {
    radius = slider[RADIUS]->value;
    brightness = slider[BRIGHTNESS]->value;
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
      if (buttons[index]->Collided(x, y))
      {
        operation = index;
        return true;
      }
    }
    return false;
  }

  int state = -1;
  void handleClick(int x1, int y1, int x2, int y2, int state)
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
    this->state = state;
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
    applySliders();
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

class MenuLayer : public Menu
{
public:
  CheckBox **checkBox;
  Button *buttonAddLayer;
  Button **buttonsAlter;
  bool swapUp;
  bool swapDown;

  MenuLayer(Coordinates coords) : Menu(coords, 0)
  {
    this->coords = coords;
    swapUp = false;
    swapDown = false;
    Coordinates drawArea(DRAW_AREA_X1, DRAW_AREA_Y1, DRAW_AREA_X2, DRAW_AREA_Y2);
    layerManager = new LayerManager(drawArea);
    controlButtons();
  }

  void controlButtons()
  {
    const int addLayerOffsetX = 60;
    const int addLayerOffsetY = 10;

    const int alterButtonsOffsetX = -50;
    const int alterButtonsStartY = 280;
    const int alterButtonsSpacingY = 40;

    buttonAddLayer = new Button(coords.x1 + addLayerOffsetX, coords.y1 + addLayerOffsetY, BUTTON_SIZE, BUTTON_SIZE);

    buttonsAlter = new Button *[2];
    for (int i = 0; i < 2; i++)
    {
      int y = alterButtonsStartY + i * alterButtonsSpacingY;
      buttonsAlter[i] = new Button(coords.x1 + alterButtonsOffsetX, y, BUTTON_SIZE, BUTTON_SIZE);
    }
    buttonAddLayer->setImage(imgButtons[0].c_str());
    buttonsAlter[0]->setImage(imgButtons[1].c_str());
    buttonsAlter[1]->setImage(imgButtons[2].c_str());
  }

  // Criar botões e checkbox
  void createLayer(int nNewButtons)
  {
    int baseX = coords.x1 + 10;
    int baseY = coords.y1 + 10;
    int buttonSize = 30;
    int spacing = 10;
    int checkboxSize = 15;

    for (int i = 0; i < nNewButtons; i++)
    {
      int index = numButtons + i;

      int bx = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseX + index * (buttonSize + spacing) : baseX;
      int by = (coords.x2 - coords.x1 > coords.y2 - coords.y1) ? baseY : baseY + index * (buttonSize + spacing);

      Coordinates coordsCheckBox(bx - checkboxSize - spacing * 2, by + spacing / 2, bx - spacing * 2, by + spacing / 2 + checkboxSize);

      checkBox[index] = new CheckBox(coordsCheckBox);
      buttons[index] = new Button(bx, by, BUTTON_SIZE, BUTTON_SIZE);

      layerManager->addLayer(img[index].c_str(), 50, 0);
      if (!checkBox[layerManager->getActiveLayer()]->isChecked())
      {
        checkBox[layerManager->getActiveLayer()]->setCheck(true);
      }
      layerManager->setActiveLayer(i);
    }
  }

  void createButtons() override
  {
    buttons = new Button *[MAX_LAYERS];
    checkBox = new CheckBox *[MAX_LAYERS];
  }

  void addButton()
  {
    if (numButtons < img.size())
    {
      createLayer(1);
      numButtons++;
    }
    else
    {
      std::cout << "Limite de camadas excedida, adicione mais imagens no array para mais camada!";
    }
  }

  void swapCheckButton(int i, int j)
  {
    if (!checkBox[i] || !checkBox[j] || !buttons[i] || !buttons[j])
    {
      std::cout << "Erro: botão ou checkbox nulo em swapCheckButton (" << i << ", " << j << ")\n";
      return;
    }

    Coordinates posCheckI = checkBox[i]->getPositon();
    Coordinates posCheckJ = checkBox[j]->getPositon();
    int xI = buttons[i]->x, yI = buttons[i]->y;
    int xJ = buttons[j]->x, yJ = buttons[j]->y;

    // Trocar ponteiros
    std::swap(checkBox[i], checkBox[j]);
    std::swap(buttons[i], buttons[j]);

    // Atualizar posições visuais
    checkBox[i]->setPosition(posCheckI);
    checkBox[j]->setPosition(posCheckJ);

    buttons[i]->setPosition(xI, yI);
    buttons[j]->setPosition(xJ, yJ);
  }

  void renderButtons() override
  {
    for (int i = 0; i < numButtons; i++)
    {
      buttons[i]->Render();
      checkBox[i]->render();
    }
    buttonAddLayer->Render();
    buttonsAlter[0]->Render();
    buttonsAlter[1]->Render();
  }

  virtual bool collisionButtons(int x, int y)
  {
    for (int index = 0; index < numButtons; index++)
    {
      if (buttons[index] && buttons[index]->Collided(x, y))
      {
        layerManager->setActiveLayer(index);
        attBrightness = true;
        return true;
      }
      if (checkBox[index] && checkBox[index]->click(x, y))
      {
        disableLayer = index;
        return true;
      }
    }

    if (buttonAddLayer && buttonAddLayer->Collided(x, y))
    {
      addButton();
      return true;
    }

    if (buttonsAlter[0]->Collided(x, y))
    {
      swapUp = true;
      return true;
    }
    else if (buttonsAlter[1]->Collided(x, y))
    {
      swapDown = true;
      return true;
    }
    return false;
  }
  void buttonsSetColor()
  {
    const float active_r = 1.0;
    const float active_g = 0.0;
    const float active_b = 0.0;

    for (int i = 0; i < numButtons; i++)
    {
      if (i == layerManager->getActiveLayer())
      {
        buttons[i]->setColor(active_r, active_g, active_b);
      }
      else
      {
        buttons[i]->setDefaultColor();
      }
    }
  }

  LayerManager *getLayerManager()
  {
    return layerManager;
  }

  bool shouldUpdateBrightness()
  {
    return attBrightness;
  }
  void resetBrightnessFlag()
  {
    attBrightness = false;
  }

  void functions() override
  {

    if (disableLayer != -1)
    {
      layerManager->toggleLayerVisibility(disableLayer);
      disableLayer = -1;
      return;
    }

    int active = layerManager->getActiveLayer();

    if (active >= 0 && active < numButtons && checkBox[active])
    {
      buttonsSetColor();
      checkBox[active]->setCheck(true);

      if (swapUp)
      {
        if (active > 0)
        {
          std::swap(layerManager->layers[active], layerManager->layers[active - 1]);
          swapCheckButton(active, active - 1);
          layerManager->setActiveLayer(active - 1);
          swapUp = false;
        }
      }
      else if (swapDown)
      {
        if (active < layerManager->layers.size() - 1)
        {
          std::swap(layerManager->layers[active], layerManager->layers[active + 1]);
          swapCheckButton(active, active + 1);
          layerManager->setActiveLayer(active + 1);
          swapDown = false;
        }
      }
    }
  }
};

class MenuFunctions : public Menu
{
  MenuLayer *menuLayer;

public:
  MenuFunctions(Coordinates coords, int numButtons) : Menu(coords, numButtons) {}

  void setLayerManager(LayerManager *lm)
  {
    layerManager = lm;
  }

  void setMenuLayer(MenuLayer *menu)
  {
    menuLayer = menu;
    layerManager = menuLayer->getLayerManager();
  }

  void sliderExtremites()
  {
    slider[RADIUS]->setExtremities(0.0, 100, 1.0);
    slider[BRIGHTNESS]->setExtremities(0.0, 5, 1.0);
  }

  void init() override
  {
    createButtons();
    createSliders();
    sliderExtremites();
    buttonsFunctionsSetImage();
    applySliders();
    setColor();
  }

  void buttonsFunctionsSetImage()
  {
    int offset = 3;
    for (int i = 0; i < numButtons; i++)
    {
      int index = i + offset;
      if (index < imgButtons.size())
      {
        buttons[i]->setImage(imgButtons[index].c_str());
      }
    }
  }

  void render()
  {
    CV::color(MENU_COLOR_R, MENU_COLOR_G, MENU_COLOR_B);      // Cor preta;
    CV::rectFill(coords.x1, coords.y1, coords.x2, coords.y2); // Criar menu;
    renderButtons();
    renderSliders();
  }
  void renderSliders()
  {
    renderSliderRGB();
    renderLabeledSlider(RADIUS, "RAIO", TEXT_OFFSET_RADIUS);
    renderLabeledSlider(BRIGHTNESS, "BRILHO", TEXT_OFFSET_BRIGHTNESS);
  }

  void renderLabeledSlider(int sliderIndex, const char *label, int labelOffsetX)
  {
    CV::color(1, 1, 1);
    int yPos = SLIDER_START_Y + SLIDER_SPACING_Y * sliderIndex;
    int textX = SLIDER_INDICATOR_X1 - labelOffsetX;
    CV::text(textX, yPos + 5, label);
    slider[sliderIndex]->render();
  }

  void renderSliderRGB()
  {
    CV::color(color.r, color.g, color.b);
    CV::rectFill(COLOR_PREVIEW_X1, COLOR_PREVIEW_Y1, COLOR_PREVIEW_X2, COLOR_PREVIEW_Y2);
    for (int i = 0; i <= B; i++)
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

  void notOperating()
  {
    operation = -1;
  }

  void flip(FlipType flip)
  {
    Bmp *image;
    image = layerManager->layers[layerManager->getActiveLayer()]->image;
    FlipType currentFlip = image->getFlip();

    switch (currentFlip)
    {
    case NONE:
      image->setFlip(flip);
      break;
    case FLIP_VERTICAL:
      image->setFlip(flip == FLIP_VERTICAL ? NONE : FLIP_BOTH);
      break;
    case FLIP_HORIZONTAL:
      image->setFlip(flip == FLIP_HORIZONTAL ? NONE : FLIP_BOTH);
      break;
    case FLIP_BOTH:
      image->setFlip(flip == FLIP_VERTICAL ? FLIP_HORIZONTAL : FLIP_VERTICAL);
      break;
    default:
      break;
    }
  }

  void load_brightness()
  {
    Bmp *image = layerManager->layers[layerManager->getActiveLayer()]->image;
    image->setBrightness(brightness);
  }

  void updateBrightness(int index)
  {
    if (menuLayer->shouldUpdateBrightness())
    {
      slider[BRIGHTNESS]->setValue(layerManager->layers[index]->image->getBrightnesse());
      menuLayer->resetBrightnessFlag();
    }
    else
    {
      load_brightness();
    }
  }

  void functions() override
  {

    setColor();
    applySliders();

    int activeLayer = layerManager->getActiveLayer();
    if (!layerManager || activeLayer == -1)
    {
      std::cout << "\nNenhuma camada selecionada!";
      return;
    }

    updateBrightness(activeLayer);

    if (operation == -1)
    {
      return;
    }

    if (state == -2)
    {
      switch (operation)
      {
      case 0:
        layerManager->layerActive()->addCircle(mouseCoords.x1, mouseCoords.y1, radius, color.r, color.g, color.b);
        break;
      case 1:
        layerManager->layerActive()->addLine(mouseCoords, color.r, color.g, color.b);
        break;
      case 2:
        layerManager->layerActive()->removeElement(mouseCoords.x2, mouseCoords.y2);
        break;

      default:
        break;
      }
    }
    else
    {
      switch (operation)
      {
      case 3:
        radius = sqrt(pow(mouseCoords.x2 - mouseCoords.x1, 2) + pow(mouseCoords.y2 - mouseCoords.y1, 2)) / 2;
        layerManager->layerActive()->addCircle(mouseCoords.x1, mouseCoords.y1, radius, color.r, color.g, color.b);
        break;
      case 4:
        layerManager->layerActive()->addRect(mouseCoords, color.r, color.g, color.b);
        break;
      case 5:
        flip(FLIP_VERTICAL);
        operation = -1;
        break;
      case 6:
        flip(FLIP_HORIZONTAL);
        operation = -1;
        break;
      default:
        break;
      }
    }
  }
};

#endif
