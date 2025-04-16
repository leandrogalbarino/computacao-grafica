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
  bool visible;   // Se a camada está visível
  int posX, posY; // Posição na tela
  Coordinates coords;
  Storage *storage;

  Layer(const char *fileName, int posX, int posY, Coordinates coords) : coords(coords)
  {
    image = nullptr;
    this->posX = posX;
    this->posY = posY;

    visible = true;
    storage = new Storage(coords);
    this->fileName = fileName;
    image = new Bmp(fileName);

    if (image != nullptr)
    {
      std::cout << "imagem adicionada!";
      image->setPosition(coords.x1 + posX, coords.y1 + posY);
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
