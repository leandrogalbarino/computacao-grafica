#ifndef LAYER_H
#define LAYER_H

#include "gl_canvas2d.h"
#include "bmp.h"
#include <string>
#include <iostream>

class Layer {
public:
    // unsigned char* pixels; // Dados da imagem
  Bmp *image;
  int width, height;
  bool visible; // Se a camada está visível
  int x, y;     // Posição na tela

  Layer(const char *fileName, int posX, int posY)
  {
    image = nullptr;
    x = posX;
    y = posY;
    visible = true;

    image = new Bmp(fileName);
    if(image != nullptr){
      std::cout << "imagem adicionada!";
      image->setPosition(posX, posY);
      image->convertBGRtoRGB();
      image->getImage();
    }
  }

  void setPostion(int posX, int posY){
    if(image != nullptr){
      image->setPosition(posX, posY);
    }
  }
    
  void draw(){
    if(visible && image){
      image->render();
    }
  }

  void toggleVisibility(){
    visible = !visible;
  }
};

#endif
