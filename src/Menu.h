#ifndef __MENU_H__
#define __MENU_H__

#include "gl_canvas2d.h"
#include "botao.h"
#include "armazenamento.h"
#include <cmath>  // Necessário para sqrt() e pow()
#include "LayerManager.h"
#include <string.h>
// #define NUM_BUTTONS 10

typedef struct{
    int x;
    int y;
    int _x;
    int _y;
} Coordinates;

typedef struct{
  float r;
  float g;
  float b;
} Color;

std::string img[] = {
  "t1/images/img3.bmp",
  "t1/images/img1.bmp",
  "t1/images/img2.bmp"
};

class Menu{
protected:
  int operation;
  int posX;
  int posY;
  int posX2;
  int posY2;
  Botao** buttons;
  int numButtons;
  Coordinates coord;
  Color color;
  // Armazenamento *armazenamento;
  // armazenamento = new Armazenamento(20);
  // armazenamento->renderizarFormas();



public:
    Menu(int posX, int posY,int posX2, int posY2, int numButtons)
    {
      if(numButtons < 3){
        numButtons = 3;
      }

      this->posX = posX;
      this->posY = posY;
      this->posX2 = posX2;
      this->posY2 = posY2;
      this->numButtons = numButtons;

      color.r = color.g = color.b = 0;
      coord._x = coord._y = coord.x = coord.y = 0;
      operation = -1;

      buttons = new Botao *[numButtons];

      int baseX = posX + 10;
      int baseY = posY + 10;
      int buttonSize = 30;
      int spacing = 10;

      for (int i = 0; i < numButtons; i++)
      {
        int bx = (posX2 - posX > posY2 - posY ) ? baseX + i * (buttonSize + spacing): baseX;
        int by = (posX2 - posX > posY2 - posY) ? baseY : baseY + i *(buttonSize + spacing);
        buttons[i] = new Botao(bx, by, 30, 30);
      }
    }

    void setColor(int r, int g, int b){
      color.r = r/255;
      color.g = g/255;
      color.b = b/255;
    }

    void renderButtons() {
      for (int i = 0; i < numButtons; i++) {
          buttons[i]->Render();
      }
    }

    void collisionButtons(int x,int y){
        for(int index = 0; index < numButtons;index++){
            if(buttons[index]->Colidiu(x,y)){
                operation = index;
                functions();
            }
        }
    }

    virtual void render(){
        CV::color(color.r,color.g,color.b);
        CV::rectFill(posX, posY, posX2, posY2); // Criar menu;
        renderButtons();

        // armazenamento->renderizarFormas();
    }

    void setCoord(int x, int y,int _x, int _y){
        coord.x = x;
        coord.y = y;
        coord._x = _x;
        coord._y = _y;
    }
    virtual void functions(){}

    // Função para apagar elementos (exemplo)
    void apagar() {
        CV::clear(1, 1, 1);  // Limpa a tela (branco)
    }
};

class MenuTop : public Menu{

  
  void functions() override{
    float raio;
    float centroX;
    float centroY;
    switch (operation)
    {
        case -1:
          break;
        case 0:
            // armazenamento->adicionarRetangulo(coordenadas.x, coordenadas.y, coordenadas._x, coordenadas._y, 1, 0, 0);
          break;
        case 1:
            // raio = sqrt(pow(coordenadas._x - coordenadas.x, 2) + pow(coordenadas._y - coordenadas.y, 2)) / 2;
            // armazenamento->adicionarCirculo(coordenadas.x, coordenadas.y, raio, 0, 1, 1 );
          break;
        case 2:
          break;
        default:
          break;
    }
  }
};

class MenuLayer : public Menu{
public:
  LayerManager *layerManager;

  MenuLayer(int posX, int posY ,int posX2, int posY2, int numButtons) : Menu(posX, posY, posX2, posY2, numButtons){
    layerManager = new LayerManager();
  }

  void render() override{
    CV::color(color.r,color.g,color.b);
    CV::rectFill(posX, posY, posX2, posY2); // Criar menu;
    renderButtons();
    layerManager->drawLayers();
  }

  void functions() override{
    if(operation == -1){
      return;
    }
    if(operation == 0 || operation == 1 || operation == 2){
      layerManager->addLayer(img[operation].c_str(), 300, 300);
      return;
    }

    // switch (operation)
    // {

    // }
  }
};
#endif
