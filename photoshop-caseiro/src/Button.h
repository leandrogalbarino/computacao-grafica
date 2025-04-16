#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gl_canvas2d.h"
#include "Bmp.h"

#define DEFAULT_R 0.42
#define DEFAULT_G 0.55
#define DEFAULT_B 0.9

class Button
{
public:
  float height, width, x, y;
  float r, g, b;
  Bmp* image = nullptr; // nova linha
  int direction;

  Button(float _x, float _y, float _width, float _height)
  {
    height = _height;
    width = _width;
    x = _x;
    y = _y;
    setDefaultColor();
    //  strcpy(label, _label);
  }

  void setPosition(int x, int y)
  {
    this->x = x;
    this->y = y;
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

  void setImage(const char* filename)
  {
    if (image) delete image;
    image = new Bmp(filename);
    image->convertBGRtoRGB();
    image->getImage(); // carrega os dados da imagem
    image->setPosition(x, y); // ← importante definir onde será desenhada
  }
  

  void Render()
  {
    if (image)
    {
      image->setPosition(x, y);
      image->renderToFit(30,30); // ← desenha a imagem no botão
    }
    else
    {
      CV::color(r, g, b);
      CV::rectFill(x, y, x + width, y + height);
    }
  
    // Borda do botão
    CV::color(0, 0, 0);
    CV::rect(x, y, x + width, y + height);
  }
  


  // recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Collided(int mx, int my)
  {
    if (mx >= x && mx <= (x + width) && my >= y && my <= (y + height))
    {
      return true;
    }
    return false;
  }
};

#endif
