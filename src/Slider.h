#ifndef __SLIDER_H__
#define __SLIDER_H__
#include "gl_canvas2d.h"
#include <algorithm>
#include <string>
class Slider
{
public:
  int x1, x2, y1, y2;
  float pointer;
  float value;

  Slider(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2)
  {
    pointer = (x1 + x2) / 2;
    updateValue();
  }

  void updateValue()
  {
    value = float(pointer - x1) / float(x2 - x1);
  }

  void setPointer(int px) {
    pointer = std::max(x1, std::min(px, x2)); // trava dentro dos limites
    updateValue(); // atualiza o valor proporcional (de 0 a 1)
  }

  void render()
  {
    // Linha base
    CV::color(1.0, 1.0, 1.0);
    CV::line(x1, y1, x2, y2);
    CV::text(x1, y1 + 15, std::to_string(value).substr(0, 4).c_str());
    // Ponteiro (desenhado como um pequeno retângulo)
    CV::color(1.0, 0.0, 0.0);
    float halfHeight = 5;
    CV::rectFill(pointer - 3, y1 - halfHeight, pointer + 3, y1 + halfHeight);
  }
  // Verifica se clicou no slider
  bool isHovering(int mx, int my)
  {
    return mx >= x1 && mx <= x2 && my >= y1 - 5 && my <= y1 + 5;
  }

  // Getter para o valor (por exemplo, para usar como cor, raio, etc)
  float getValue() const
  {
    return value;
  }
};

#endif