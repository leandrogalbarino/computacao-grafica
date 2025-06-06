
// Vector3: Função auxiliar para facilitar multiplos calculos de coordenadas.

#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include "gl_canvas2d.h"

class Vector3
{
public:
    float x, y, z;

    Vector3()
    {
        x = y = z = 0;
    }

    Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
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

    Vector3 operator-(const Vector3 &v)
    {
        Vector3 aux(x - v.x, y - v.y, z - v.z);
        return (aux);
    }

    float length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3 operator+(const Vector3 &v)
    {
        Vector3 aux(x + v.x, y + v.y, z + v.z);
        return (aux);
    }

    Vector3 operator+(float sum) const
    {
        Vector3 aux(x + sum, y + sum, z + sum);
        return (aux);
    }

    // Adicionem os demais overloads de operadores aqui.
    Vector3 operator*(float esc) const
    {
        Vector3 aux(esc * x, esc * y, esc * z);
        return (aux);
    }

    Vector3 operator*(const Vector3 &v)
    {
        Vector3 aux(x * v.x, y * v.y, z * v.z);
        return (aux);
    }

    Vector3 operator/(float div)
    {
        Vector3 aux(x / div, y / div, z / div);
        return (aux);
    }

    float dot(Vector3 v2)
    {
        return (x * v2.x + y * v2.y + z * v2.z);
    }

    float modulo()
    {
        return std::sqrt(x * x + y * y + z*z);
    }
};

#endif
