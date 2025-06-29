
//Vector2: Função auxiliar para facilitar multiplos calculos de coordenadas.

#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include "gl_canvas2d.h"

class Vector2
{
public:
    float x, y;

    Vector2()
    {
        x = y = 0;
    }

    Vector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void normalize()
    {
        float norm = (float)sqrt(x * x + y * y);

        if (norm == 0.0)
        {
            printf("\n\nNormalize::Divisao por zero");
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
    }

    Vector2 operator-(const Vector2 &v)
    {
        Vector2 aux(x - v.x, y - v.y);
        return (aux);
    }
    Vector2 operator-(const Vector2 &v) const
    {
        Vector2 aux(x - v.x, y - v.y);
        return (aux);
    }

    float length() const
    {
        return sqrt(x * x + y * y);
    }
    
    Vector2 operator+(const Vector2 &v)
    {
        Vector2 aux(x + v.x, y + v.y);
        return (aux);
    }

    Vector2 operator+(float sum) const
    {
        Vector2 aux(x + sum, y + sum);
        return (aux);
    }

    // Adicionem os demais overloads de operadores aqui.
    Vector2 operator*(float esc) const
    {
        Vector2 aux(esc * x, esc * y);
        return (aux);
    }

    Vector2 operator*(const Vector2 &v)
    {
        Vector2 aux(x * v.x, y * v.y);
        return (aux);
    }

    Vector2 operator/(float div)
    {
        Vector2 aux(x / div, y / div);
        return (aux);
    }

    float dot(Vector2 v2)
    {
        return (x * v2.x + y * v2.y);
    }

    float modulo()
    {
        return std::sqrt(x * x + y * y);
    }
};

#endif
