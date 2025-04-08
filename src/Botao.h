#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

#define DEFAULT_R 0.42
#define DEFAULT_G 0.55
#define DEFAULT_B 0.9

class Botao
{
public:
  float altura, largura, x, y;
  float r, g, b;
  Botao(float _x, float _y, float _larg, float _alt)
  {
    altura = _alt;
    largura = _larg;
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

  void Render()
  {
    CV::color(r, g, b);
    CV::rectFill(x, y, x + largura, y + altura);
    CV::color(0, 0, 0);
    // CV::text(x+5, y+altura/2, label); //escreve o label do botao mais ou menos ao centro.
  }

  // recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
    if (mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura))
    {
      return true;
    }
    return false;
  }
};

#endif
