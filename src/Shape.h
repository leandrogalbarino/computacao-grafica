#ifndef SHAPE_H
#define SHAPE_H

#include "gl_canvas2d.h"

class Shape
{
public:
  enum Type
  {
    RECTANGLE,
    CIRCLE,
    POINT,
    LINE,
    TRIANGLE
  };

private:
  Type type;

  // retângulo
  float x1, y1, x2, y2;
  float radius;

  // círculo
  
  
  // linha
  // float lx1, ly1, lx2, ly2;

  // triângulo
  float tx1, ty1, tx2, ty2, tx3, ty3;

  float r, g, b;

public:
  // Constructors
  Shape()
      : type(RECTANGLE), x1(0), y1(0), x2(0), y2(0), radius(0), r(0), g(0), b(0) {}

  Shape(Type _type, float _x1, float _y1, float _x2, float _y2, float _radius, float _r, float _g, float _b)
      : type(_type), x1(_x1), y1(_y1), x2(_x2), y2(_y2), radius(_radius), r(_r), g(_g), b(_b) {}

  // Factory methods
  static Shape createRectangle(float x1, float y1, float x2, float y2, float r, float g, float b)
  {
    return Shape(RECTANGLE, x1, y1, x2, y2, 0, r, g, b);
  }

  static Shape createCircle(float x, float y, float radius, float r, float g, float b)
  {
    return Shape(CIRCLE, x, y, 0, 0, radius, r, g, b);
  }

  static Shape createLine(float x1, float y1, float x2, float y2, float r, float g, float b)
  {
    return Shape(LINE, x1, y1, x2, y2, 0, r, g, b);
  }

  static Shape createPoint(float x1, float y1, float r, float g, float b)
  {
    return Shape(POINT, x1, y1, x1, y1, 0, r, g, b);
  }

  // Drawing function
  void render() const
  {
    CV::color(r, g, b);
    switch (type)
    {
    case RECTANGLE:
      CV::rectFill(x1, y1, x2, y2);
      break;
    case CIRCLE:
      CV::circleFill(x1, y1, radius, 20);
      break;
    case POINT:
      CV::point(x1, y1);
      break;
    case LINE:
      CV::line(x1, y1, x2, y2);
      break;
    default:
      break;
    }
  }

  bool contains(int px, int py)
  {
    switch (type)
    {
    case RECTANGLE:
      return px >= x1 && px <= x2 && py >= y1 && py <= y2;

    case CIRCLE:
    {
      float dx = px - x1;
      float dy = py - y1;
      return dx * dx + dy * dy <= radius * radius;
    }

    case LINE:
    {
      float dx = x2 - x1;
      float dy = y2 - y1;
      float lengthSquared = dx * dx + dy * dy;
      if (lengthSquared == 0)
        return false;

      float t = ((px - x1) * dx + (py - y1) * dy) / lengthSquared;
      if (t < 0 || t > 1)
        return false;

      float projX = x1 + t * dx;
      float projY = y1 + t * dy;

      float distSq = (projX - px) * (projX - px) + (projY - py) * (projY - py);
      return distSq <= 5.0f; // margem de clique, ajustável
    }

    case TRIANGLE:
    {
      auto area = [](float x1, float y1, float x2, float y2, float x3, float y3)
      {
        return fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
      };

      float A = area(tx1, ty1, tx2, ty2, tx3, ty3);
      float A1 = area(px, py, tx2, ty2, tx3, ty3);
      float A2 = area(tx1, ty1, px, py, tx3, ty3);
      float A3 = area(tx1, ty1, tx2, ty2, px, py);

      return fabs(A - (A1 + A2 + A3)) < 0.01;
    }

    default:
      return false;
    }
  }

  Type getType() const { return type; }
};
#endif