#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <vector>
#include "Layer.h"
#include "gl_canvas2d.h"

class LayerManager
{

private:
  int activeLayer;
  int x1, x2, y1, y2; // delimitações de desenho

public:
  std::vector<Layer *> layers;

  LayerManager(int x1, int y1, int x2, int y2)
  {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    activeLayer = -1;
  }

  void addLayer(const char *filename, int x, int y)
  {
    for (int i = 0; i < layers.size(); i++)
    {
      if (layers[i]->fileName == filename)
      {
        setActiveLayer(i);
        return;
      }
    }
    
    Layer *newLayer = new Layer(filename, x, y);
    if (!newLayer)
    {
      std::cerr << "Erro ao criar camada!" << std::endl;
      return;
    }
    layers.push_back(newLayer);
    setActiveLayer(layers.size() - 1);
  }

  void removeLayer(int index)
  {
  }

  void drawBackground() {
    int cellSize = 20;
    for (int y = y1; y < y2; y += cellSize) {
        for (int x = x1; x < x2; x += cellSize) {
            bool isEven = ((x / cellSize) + (y / cellSize)) % 2 == 0;

            if (isEven)
                CV::color(0.85, 0.85, 0.85); // cinza claro
            else
                CV::color(0.65, 0.65, 0.65); // cinza escuro

            CV::rectFill(x, y, x + cellSize, y + cellSize);
        }
    }
  }

  void drawLayers()
  {
    drawBackground();
    for (Layer *layer : layers)
    {
      layer->draw();
    }
  }

  int getActiveLayer()
  {
    return activeLayer;
  }

  void setActiveLayer(int index)
  {
    if (index >= 0 && index < layers.size())
    {
      activeLayer = index;
      if (!layers[index]->isVisible())
      {
        layers[index]->setVisible(true);
      }
    }
  }

  void toggleLayerVisibility(int index)
  {
    if (index >= 0 && index < layers.size())
    {
      if (index == activeLayer || activeLayer == -1)
      {
        activeLayer = (layers[index]->isVisible()) ? -1 : index;
      }

      layers[index]->toggleVisibility();
    }
  }

  Storage *layerActive()
  {
    if (activeLayer != -1)
    {
      return layers[activeLayer]->storage;
    }
    std::cout << "Nenhuma camada seleciona!";
  }
};

#endif
