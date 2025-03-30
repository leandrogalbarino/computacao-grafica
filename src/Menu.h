#ifndef __MENU_H__
#define __MENU_H__

#include "gl_canvas2d.h"
#include "botao.h"
#include "armazenamento.h"
#include <cmath>  // Necessário para sqrt() e pow()
typedef struct{
    int x_ini;
    int y_ini;
    int x_fim;
    int y_fim;
} Coordenadas;

class Menu{
int operacao;
int largura;
int altura;
int numBotoes;
Botao** botoes;
Coordenadas coordenadas;
Armazenamento *armazenamento;


public:
    Menu(int largura, int altura)
    {
        this->largura = largura;
        this->altura = altura;
        coordenadas.x_fim = coordenadas.y_fim = coordenadas.x_ini = coordenadas.y_ini = 0;
        operacao = 0;
        numBotoes = 10;
        botoes  = new Botao*[numBotoes];
        armazenamento = new Armazenamento(20);

        int x= 5, y = 5;
        for (int i = 0; i < numBotoes;i ++){
          botoes[i] = new Botao(x , i*35 + y, 30, 30);
        }
    }

    void funcao(int func){
        operacao = func;
    }


    void renderBotoes() {
      for (int i = 0; i < numBotoes; i++) {
          botoes[i]->Render();
      }
    }

    void colisaoBotoes(int x,int y){

        for(int i = 0; i < numBotoes;i++){
            if(botoes[i]->Colidiu(x,y)){
                funcao(i);
            }
        }
    }

    void render(){
        CV::color(0.35,0.35,0.35);
        CV::rectFill(0, 0, largura, altura);
        renderBotoes();
        armazenamento->renderizarFormas();
    }

    void setCoordenadas(int x_ini, int y_ini,int x_fim, int y_fim){
        coordenadas.x_ini = x_ini;
        coordenadas.y_ini = y_ini;
        coordenadas.x_fim = x_fim;
        coordenadas.y_fim = y_fim;
    }

    void armazenar(){
        float raio;
        float centroX;
        float centroY;
        switch (operacao)
        {
            case 0:
              break;
            case 1:
                armazenamento->adicionarRetangulo(coordenadas.x_ini, coordenadas.y_ini, coordenadas.x_fim, coordenadas.y_fim, 1, 0, 0);
              break;
            case 2:
                raio = sqrt(pow(coordenadas.x_fim - coordenadas.x_ini, 2) + pow(coordenadas.y_fim - coordenadas.y_ini, 2)) / 2;
                armazenamento->adicionarCirculo(coordenadas.x_fim, coordenadas.y_fim, raio, 0, 1, 1 );

              break;
            case 3:
              break;
            default:
              break;
        }
    }



    void criarRetangulo(){
        //std::cout << "Criando um retângulo.." << std :: env1;
        CV::color(1, 0, 0);  // Cor vermelha
        CV::rectFill(coordenadas.x_ini, coordenadas.y_ini, coordenadas.x_fim, coordenadas.y_fim);
    }
    // Função para criar um círculo
    void criarCirculo() {
      //std::cout << "Criando um círculo..." << std::endl;
      CV::color(0, 1, 0);  // Cor verde
      CV::circleFill(100, 100, 30, 20);
    }

      // Função para apagar elementos (exemplo)
    void apagar() {
        CV::clear(1, 1, 1);  // Limpa a tela (branco)
    }

    void executar(){
        switch (operacao)
        {
            case 0:
              break;
            case 1:
              criarRetangulo();
              return;
            case 2:
              criarCirculo();
              return;
            case 3:
              apagar();
              return;
            default:
              break;
        }
    }
};

#endif
