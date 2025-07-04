// Coord.h
// Define a estrutura de coordenadas em 3D utilizada para representar posições e direções no espaço.
#ifndef __COORD_H__
#define __COORD_H__

class Coord
{
public:
  float x;
  float y;
  float z;
  Coord(){
    
  }
  Coord(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

#endif