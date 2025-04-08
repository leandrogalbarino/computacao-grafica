#ifndef SHAPE_H
#define SHAPE_H

#include "gl_canvas2d.h"
#include "Coordinates.h"
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

  Coordinates coords;
  float radius;
  // triângulo
  float tx1, ty1, tx2, ty2, tx3, ty3;

  float r, g, b;

public:
  // Constructors

  Shape(Type _type, Coordinates c, float _radius, float _r, float _g, float _b)
      : type(_type), coords(c), radius(_radius), r(_r), g(_g), b(_b) {}

  // Factory methods
  static Shape createRectangle(Coordinates c, float r, float g, float b)
  {
    return Shape(RECTANGLE, c, 0, r, g, b);
  }

  static Shape createLine(Coordinates c, float r, float g, float b)
  {
    return Shape(LINE, c, 0, r, g, b);
  }

  static Shape createCircle(float x, float y, float radius, float r, float g, float b)
  {
    return Shape(CIRCLE, Coordinates(x, y, 0, 0), radius, r, g, b);
  }

  static Shape createPoint(float x1, float y1, float r, float g, float b)
  {
    return Shape(POINT, Coordinates(x1, y1, x1, y1), 0, r, g, b);
  }

  // Drawing function
  void render() const
  {
    CV::color(r, g, b);
    switch (type)
    {
    case RECTANGLE:
      CV::rectFill(coords.x1, coords.y1, coords.x2, coords.y2);
      break;
    case CIRCLE:
      CV::circleFill(coords.x1, coords.y1, radius, 20);
      break;
    case POINT:
      CV::point(coords.x1, coords.y1);
      break;
    case LINE:
      CV::line(coords.x1, coords.y1, coords.x2, coords.y2);
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
      return px >= coords.x1 && px <= coords.x2 && py >= coords.y1 && py <= coords.y2;

    case CIRCLE:
    {
      float dx = px - coords.x1;
      float dy = py - coords.y1;
      return dx * dx + dy * dy <= radius * radius;
    }

    case LINE:
    {
      float dx = coords.x2 - coords.x1;
      float dy = coords.y2 - coords.y1;
      float lengthSquared = dx * dx + dy * dy;
      if (lengthSquared == 0)
        return false;

      float t = ((px - coords.x1) * dx + (py - coords.y1) * dy) / lengthSquared;
      if (t < 0 || t > 1)
        return false;

      float projX = coords.x1 + t * dx;
      float projY = coords.y1 + t * dy;

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