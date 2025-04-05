#ifndef __INTERFACEMANAGER_H__
#define __INTERFACEMANAGER_H__
#include "botao.h"
#include "menu.h"

class InterfaceManeger{
public:
  Botao **buttons;
  Menu *menuSide;
  Menu *menuTop;

  InterfaceManeger(){
    menuSide = new Menu(0, 0, 50, 720);
    menuTop = new Menu(40, 0, 1250, 50);
  }

  
};

#endif