#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <vector>
#include "Layer.h"
#include "gl_canvas2d.h"
#include "Coordinates.h"
#include <iostream>


class LayerManager
{

private:
  int activeLayer;
  Coordinates coords;

public:
  std::vector<Layer *> layers;

  LayerManager(Coordinates coords) : coords(coords)
  {
    activeLayer = -1;
  }

  void addLayer(const char *filename, int posX, int posY)
  {
    for (int i = 0; i < layers.size(); i++)
    {
      if (strcmp(layers[i]->fileName, filename) == 0)
      {
        setActiveLayer(i);
        return;
      }
    }

    Layer *newLayer = new Layer(filename, posX, posY, coords);
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

  void drawBackground()
  {
    int numDivisoes = 20; // por exemplo
    int cellSize = (coords.x2 - coords.x1) / numDivisoes;

    for (int y = coords.y1; y < coords.y2; y += cellSize)
    {
      for (int x = coords.x1; x < coords.x2; x += cellSize)
      {
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
    return nullptr;
  }
};

#endif
