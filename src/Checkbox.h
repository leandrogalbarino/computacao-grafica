#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "gl_canvas2d.h"
#include "bmp.h"
#include "Coordinates.h"

class CheckBox
{
  bool check;
  Coordinates coords;
  
  public:
  CheckBox(Coordinates coords) : coords(coords)
  {
    check = false;
  }
  void render()
  {
    if (check == true)
    {
      CV::color(0, 1, 0);
    }
    else
    {
      CV::color(1, 1, 1);
    }
    CV::rectFill(coords.x1, coords.y1, coords.x2, coords.y2);
  }

  void setPosition(Coordinates newCoords)
  {
    coords = newCoords;
  }

  Coordinates getPositon(){
    return coords;
  }

  void setCheck(bool value)
  {
    check = value;
  }

  bool click(float x, float y)
  {
    if (x >= coords.x1 && x <= coords.x2 && y >= coords.y1 && y <= coords.y2)
    {
      check = !check;
      return true;
    }
    return false;
  }

  bool isChecked() const { return check; }
};

#endif
