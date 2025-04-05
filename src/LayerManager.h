#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <vector>
#include "Layer.h"

class LayerManager {
public:
    std::vector<Layer*> layers;
    int activeLayer;

    LayerManager(){
      activeLayer = -1;
    }    

    void addLayer(const char* filename, int x, int y){
      Layer *newLayer = new Layer(filename, x, y);
      if (!newLayer) {
        std::cerr << "Erro ao criar camada!" << std::endl;
        return;
      }  
      layers.push_back(newLayer);
    }

    void removeLayer(int index){
    }

    void drawLayers(){
      for (Layer* layer : layers) {
        layer->draw();
      }
    }

    void setActiveLayer(int index){
      if (index >= 0 && index < layers.size()) {
        activeLayer = index;
      }
    }

    void toggleLayerVisibility(int index){
      if (index >= 0 && index < layers.size()) {
        layers[index]->toggleVisibility();
      }
    }
};

#endif
