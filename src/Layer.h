#ifndef LAYER_H
#define LAYER_H

#include "gl_canvas2d.h"
#include "bmp.h"
#include <string>
#include <iostream>
#include "storage.h"

class Layer
{
public:
  Bmp *image;
  const char *fileName;
  int width, height;
  bool visible; // Se a camada está visível
  int posX, posY;     // Posição na tela
  int x, y;     // Posição na tela
  int _x, _y;
  Storage *storage;

  Layer(const char *fileName, int posX, int posY)
  {
    image = nullptr;
    this->posX = posX;
    this->posY = posY;
    x = 100;
    y = 50;
    _x = 1150;
    _y = 650;
    visible = true;
    storage = new Storage(x,y,_x,_y);
    image = new Bmp(fileName);
    this->fileName = fileName;

    if (image != nullptr)
    {
      std::cout << "imagem adicionada!";
      image->setPosition(x + posX, y + posY);
      image->convertBGRtoRGB();
      image->getImage();
    }
  }

  bool isVisible()
  {
    return visible;
  }
  void setVisible(bool visible)
  {
    this->visible = visible;
  }

  void setPostion(int posX, int posY)
  {
    if (image != nullptr)
    {
      image->setPosition(posX, posY);
    }
  }

  void draw()
  {
    if (visible)
    {
      if (image)
      {
        image->render();
      }
      storage->renderShapes();
    }
  }

  void toggleVisibility()
  {
    visible = !visible;
  }
};

#endif
