#ifndef LAYER_H
#define LAYER_H

#include "gl_canvas2d.h"
#include "bmp.h"
#include <string>
#include <iostream>
#include "storage.h"

class Layer {
public:
    // unsigned char* pixels; // Dados da imagem
  Bmp *image;
  const char* fileName;
  int width, height;
  bool visible; // Se a camada está visível
  int x, y;     // Posição na tela
  Storage *storage;

  Layer(const char *fileName, int posX, int posY)
  {
    image = nullptr;
    x = posX;
    y = posY;
    visible = true;
    storage = new Storage(20);
    image = new Bmp(fileName);
    this->fileName = fileName;

    if(image != nullptr){
      std::cout << "imagem adicionada!";
      image->setPosition(posX, posY);
      image->convertBGRtoRGB();
      image->getImage();
    }
  }

  bool isVisible(){
    return visible;
  }
  void setVisible(bool visible){
    this->visible = visible;
  }

  void setPostion(int posX, int posY){
    if(image != nullptr){
      image->setPosition(posX, posY);
    }
  }
    
  void draw(){
    if(visible){
      if(image){
        image->render();
      }
      storage->renderShapes();
    }
  }

  void toggleVisibility(){
    visible = !visible;
  }
};

#endif
