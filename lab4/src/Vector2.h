#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

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

    // void drawVector(int tam)
    // {
    //     CV::color(1, 0, 0);
    //     CV::line(0, 0, x*tam, y*tam);
    // }

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

    Vector2 operator+(const Vector2 &v)
    {
        Vector2 aux(x + v.x, y + v.y);
        return (aux);
    }

    // Adicionem os demais overloads de operadores aqui.
    Vector2 operator*(int escalar)
    {
        Vector2 aux(escalar * x, escalar * y);
        return (aux);
    }

    Vector2 operator*(const Vector2 &v)
    {
        Vector2 aux(x * v.x, y * v.y);
        return (aux);
    }

    Vector2 operator/(int div)
    {
        Vector2 aux(x / div, y / div);
        return (aux);
    }

    float modulo()
    {
        return std::sqrt(x * x + y * y);
    }
};

#endif
