#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "gl_canvas2d.h"
#include "bmp.h"

class CheckBox{
  float altura, largura, x, y;
    bool check;
public:
  CheckBox(){
     altura  = 15;
     largura = 15;
     x = 0;
     y = 0;
     check = true;
  }

  void setPosition(float _x, float _y){
     x = _x;
     y = _y;
  }

  void render()
  {
      CV::color(0,0,0);
      CV::rect(x, y, x + largura, y + altura);
      CV::color(0, 0, 0);
      if(check == true){
        CV::text(x+4,y+12,"X");
      }
  }

  void click(float _x, float _y){
    if(_x > x && _x < x+largura  && _y > y &&  _y < y+altura){
        check = (check == false) ? true : false;
    }
  }


};


#endif
