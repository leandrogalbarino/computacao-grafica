#ifndef __COORDINATES_H__
#define __COORDINATES_H__

struct Coordinates {
  int x1, y1, x2, y2;

  Coordinates(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0)
      : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

#endif