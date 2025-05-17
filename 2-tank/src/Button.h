#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gl_canvas2d.h"
#include <string>

#define DEFAULT_R 0.42
#define DEFAULT_G 0.55
#define DEFAULT_B 0.9
#define WHITE 13
class Button
{
public:
  float x1, y1, x2, y2;
  float r, g, b;
  int direction;
  std::string text;

  Button()
  {
    setDefaultColor();
  }

  void setPosition(float x1, float y1, float x2, float y2)
  {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
  }

  void setMensage(const char *text)
  {
    this->text = text;
  }

  void setDefaultColor()
  {
    r = DEFAULT_R;
    g = DEFAULT_G;
    b = DEFAULT_B;
  }

  void setColor(float r, float g, float b)
  {
    this->r = r;
    this->g = g;
    this->b = b;
  }

void render()
{
    CV::color(r, g, b);
    CV::rectFill(x1, y1, x2, y2);
    CV::color(WHITE);

    float centerX = (x1 + x2) / 2.0f;
    float centerY = (y1 + y2) / 2.0f;
    int textLength = text.length() * CV::getPixelsTextSize();

    CV::text(centerX - textLength / 2, centerY, text.c_str());
}

  // recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool collided(int mx, int my)
  {
    if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2)
    {
      return true;
    }
    return false;
  }
};

#endif
