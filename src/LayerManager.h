#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <vector>
#include "Layer.h"

class LayerManager
{

private:
  int activeLayer;

public:
  std::vector<Layer *> layers;

  LayerManager()
  {
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

  void drawLayers()
  {
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

  Storage *addElements()
  {
    if (activeLayer != -1)
    {
      return layers[activeLayer]->storage;
    }
    std::cout << "Nenhuma camada seleciona!";
  }
};

#endif
