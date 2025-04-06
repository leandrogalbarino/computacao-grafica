#ifndef STORAGE_H
#define STORAGE_H

#include "gl_canvas2d.h"
#include <vector>
#include <algorithm>
#include "Shape.h"

class Storage
{
private:
    std::vector<Shape> shapes;
    int x1, y1, x2, y2;

public:
    Storage(int x1, int y1, int x2, int y2)
    {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
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

    void addRect(int x1, int y1, int x2, int y2, float r, float g, float b)
    {
        if (x1 > x2)
            std::swap(x1, x2);
        if (y1 > y2)
            std::swap(y1, y2);

        x1 = std::max(x1, this->x1);
        y1 = std::max(y1, this->y1);
        x2 = std::min(x2, this->x2);
        y2 = std::min(y2, this->y2);

        if (x1 >= x2 || y1 >= y2)
        {
            return;
        }
        Shape rect = Shape::createRectangle(x1, y1, x2, y2, r, g, b);
        addShape(rect);
    }

    void addPoint(int x1, int y1, float r,float g, float b)
    {
        // Clamping dos pontos para os limites da área
        x1 = std::max(this->x1, std::min(this->x2, x1));
        y1 = std::max(this->y1, std::min(this->y2, y1));

        Shape point = Shape::createPoint(x1, y1, r, g, b);
        addShape(point);
    }

    void addLine(int x1, int y1, int x2, int y2, float r, float g, float b)
    {
        // Clamping dos pontos para os limites da área
        x1 = std::max(this->x1, std::min(this->x2, x1));
        y1 = std::max(this->y1, std::min(this->y2, y1));
        x2 = std::max(this->x1, std::min(this->x2, x2));
        y2 = std::max(this->y1, std::min(this->y2, y2));
    
        // Se os pontos forem iguais, não é linha
        if (x1 == x2 && y1 == y2) return;
    
        Shape line = Shape::createLine(x1, y1, x2,y2, r, g, b);
        addShape(line);
    }
    

    void addCircle(float x, float y, float radius, float r, float g, float b)
    {
        float maxRaioX = std::min(x - x1, x2 - x);
        float maxRaioY = std::min(y - y1, y2 - y);
        float maxRaio = std::min(maxRaioX, maxRaioY);
        if (radius > maxRaio) {
            radius = maxRaio;
        }
        if (radius <= 0) return;

        Shape circ = Shape::createCircle(x, y, radius, r, g, b);
        addShape(circ);
    }

    bool limits(int x, int y){
        return (x >= this->x1 && y >= this->y1 && x <= this->x2 && y <= this->y2);
    }
    void removeElement(int x, int y){
        if(!limits(x,y)){
            return;
        }
        for (int i = shapes.size() - 1; i >= 0; i--){
            if (shapes[i].contains(x, y)) {
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
