#ifndef __SLIDER_H__
#define __SLIDER_H__
#include "gl_canvas2d.h"
#include <algorithm>
#include <string>
#include "Coordinates.h"

#define COLOR_BAR_R 1.0 
#define COLOR_BAR_G 1.0
#define COLOR_BAR_B 1.0

#define COLOR_POINTER_R 1.0
#define COLOR_POINTER_G 0.0
#define COLOR_POINTER_B 0.0

class Slider
{
public:
  Coordinates coords;
  float pointer;
  float value;

  Slider(Coordinates coords) : coords(coords) {
    pointer = (coords.x1 + coords.x2) / 2;
    updateValue();
  }

  void updateValue()
  {
    value = float(pointer - coords.x1) / float(coords.x2 - coords.x1);
  }

  void setPointer(int px) {
    pointer = std::max(coords.x1, std::min(px, coords.x2)); // trava dentro dos limites
    updateValue(); // atualiza o valor proporcional (de 0 a 1)
  }

  void render()
  {
    // Linha base
    float halfHeight = 5;
     
    CV::color(COLOR_BAR_R, COLOR_BAR_G, COLOR_BAR_B);
    CV::line(coords.x1, coords.y1, coords.x2, coords.y2);
    CV::text(coords.x1, coords.y1 + 15, std::to_string(value).substr(0, 4).c_str());
    
    CV::color(COLOR_POINTER_R, COLOR_POINTER_G, COLOR_POINTER_B);
    CV::rectFill(pointer - 3, coords.y1 - halfHeight, pointer + 3, coords.y1 + halfHeight);
    
  }
  // Verifica se clicou no slider
  bool isHovering(int mx, int my)
  {
    return mx >= coords.x1 && mx <= coords.x2 && my >= coords.y1 - 5 && my <= coords.y1 + 5;
  }

  float getValue() const
  {
    return value;
  }
};

#endif