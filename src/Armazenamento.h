#ifndef __ARMAZENAMENTO_H__
#define __ARMAZENAMENTO_H__

#include "gl_canvas2d.h"
#include <vector>
struct Forma {
    enum Tipo { RETANGULO, CIRCULO } tipo;
    float x1, y1, x2, y2; // Coordenadas para ret�ngulo
    float raio;           // Para c�rculo
    float r, g, b;        // Cor da forma


        Forma() : tipo(RETANGULO), x1(0), y1(0), x2(0), y2(0), raio(0), r(0), g(0), b(0) {}

    // Construtor privado para evitar ambiguidade
    Forma(Tipo _tipo, float _x1, float _y1, float _x2, float _y2, float _raio, float _r, float _g, float _b)
        : tipo(_tipo), x1(_x1), y1(_y1), x2(_x2), y2(_y2), raio(_raio), r(_r), g(_g), b(_b) {}

    // M�todos est�ticos para criar formas
    static Forma criarRetangulo(float x1, float y1, float x2, float y2, float r, float g, float b) {
        return Forma(RETANGULO, x1, y1, x2, y2, 0, r, g, b);
    }

    static Forma criarCirculo(float x, float y, float raio, float r, float g, float b) {
        return Forma(CIRCULO, x, y, 0, 0, raio, r, g, b);
    }
    void render() const {
        CV::color(r, g, b); // Define a cor da forma
        if (tipo == RETANGULO) {
            CV::rectFill(x1, y1, x2, y2); // Desenha o ret�ngulo
        } else if (tipo == CIRCULO) {
            CV::circleFill(x1, y1, raio, 20); // Desenha o c�rculo
        }
    }
};


class Armazenamento{
//;std::vector<Forma> formas;
Forma *formas;
int quantidade;
int capacidade;



public:
    // Renderizar todas as formas armazenadas
    Armazenamento(int capMax){
        capacidade = capMax;
        quantidade = 0;
        formas = new Forma[capacidade];
    }

    void renderizarFormas() const {
        for (int i = 0; i < quantidade;i++) {
            formas[i].render();
        }
    }

    void adicionarForma(Forma forma) {
        if (quantidade < capacidade) {
            formas[quantidade] = forma;
            quantidade++;
        } else {
            printf("Capacidade m�xima atingida.\n");
        }
    }


    // M�todos para adicionar formas espec�ficas
    void adicionarRetangulo(float x1, float y1, float x2, float y2, float r, float g, float b) {
        Forma ret =  (Forma::criarRetangulo(x1, y1, x2, y2, r, g, b));
        adicionarForma(ret);
    }

    void adicionarCirculo(float x, float y, float raio, float r, float g, float b) {
        //formas.push_back(Forma::criarCirculo(x, y, raio, r, g, b));
        Forma circ =(Forma::criarCirculo(x, y, raio, r, g, b));
        adicionarForma(circ);

    }

};


#endif
