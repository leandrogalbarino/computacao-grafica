#ifndef STORAGE_H
#define STORAGE_H

#include "gl_canvas2d.h"
#include <vector>
#include <algorithm>
#include "Shape.h"
#include "Coordinates.h"

class Storage
{
private:
    std::vector<Shape> shapes;
    Coordinates coords;

public:
    Storage(Coordinates coords) : coords(coords)
    {
    }

    void renderShapes() const
    {
        for (const Shape &shape : shapes)
        {
            shape.render();
        }
    }

    void addShape(const Shape &shape)
    {
        shapes.push_back(shape);
    }

    void addRect(Coordinates coordsRet, float r, float g, float b)
    {
        if (coordsRet.x1 > coordsRet.x2)
            std::swap(coordsRet.x1, coordsRet.x2);
        if (coordsRet.y1 > coordsRet.y2)
            std::swap(coordsRet.y1, coordsRet.y2);

        coordsRet.x1 = std::max(coordsRet.x1, coords.x1);
        coordsRet.y1 = std::max(coordsRet.y1, coords.y1);
        coordsRet.x2 = std::min(coordsRet.x2, coords.x2);
        coordsRet.y2 = std::min(coordsRet.y2, coords.y2);

        if (coordsRet.x1 >= coordsRet.x2 || coordsRet.y1 >= coordsRet.y2)
        {
            return;
        }
        Shape rect = Shape::createRectangle(coordsRet, r, g, b);
        addShape(rect);
    }

    
    void addLine(Coordinates coordLine, float r, float g, float b)
    {
        // Clamping dos pontos para os limites da área
        coordLine.x1 = std::max(coords.x1, std::min(coords.x2, coordLine.x1));
        coordLine.y1 = std::max(coords.y1, std::min(coords.y2, coordLine.y1));
        coordLine.x2 = std::max(coords.x1, std::min(coords.x2, coordLine.x2));
        coordLine.y2 = std::max(coords.y1, std::min(coords.y2, coordLine.y2));
        
        // Se os pontos forem iguais, não é linha
        if (coordLine.x1 == coordLine.x2 && coordLine.y1 == coordLine.y2)
        return;
        
        Shape line = Shape::createLine(coordLine, r, g, b);
        addShape(line);
    }
    
    void addPoint(int x1, int y1, float r, float g, float b)
    {
        // Clamping dos pontos para os limites da área
        x1 = std::max(coords.x1, std::min(coords.x2, x1));
        y1 = std::max(coords.y1, std::min(coords.y2, y1));

        Shape point = Shape::createPoint(x1, y1, r, g, b);
        addShape(point);
    }
    void addCircle(float x, float y, float radius, float r, float g, float b)
    {
        float maxRaioX = std::min(x - coords.x1, coords.x2 - x);
        float maxRaioY = std::min(y - coords.y1, coords.y2 - y);
        float maxRaio = std::min(maxRaioX, maxRaioY);
        if (radius > maxRaio)
        {
            radius = maxRaio;
        }
        if (radius <= 0)
            return;

        Shape circ = Shape::createCircle(x, y, radius, r, g, b);
        addShape(circ);
    }

    bool limits(int x, int y)
    {
        return (x >= coords.x1 && y >= coords.y1 && x <= coords.x2 && y <= coords.y2);
    }
    void removeElement(int x, int y)
    {
        if (!limits(x, y))
        {
            return;
        }
        for (int i = shapes.size() - 1; i >= 0; i--)
        {
            if (shapes[i].contains(x, y))
            {
                shapes.erase(shapes.begin() + i);
                break;
            }
        }
    }

    int getShapeCount() const
    {
        return shapes.size();
    }

    void clear()
    {
        shapes.clear();
    }
};

#endif
