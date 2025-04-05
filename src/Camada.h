// #ifndef __CAMADA_H__
// #define __CAMADA_H__

// #include "gl_canvas2d.h"
// #include "bmp.h"
// #include <string>
// #include <iostream>

// class Camada{
// private:
//     std::string converterParaURL(const std::string& caminho) {
//         std::string url = "";
//         for (char c : caminho) {
//             if (c == '\\') {
//                 url += '/';  // Converte barras invertidas para barras normais
//             } else {
//                 url += c;
//             }
//         }
//         return url;
//     }

// public:

//     Bmp *img;
//     unsigned char *data;

//     Camada(){
//         img = nullptr;
//         data = nullptr;
//     }

//     std::string selecionarImg(){
//         std::string caminhoArquivo;

//         std::cout << "Digite o caminho completo do arquivo: ";
//         std::getline(std::cin, caminhoArquivo);


//         std::string urlArquivo = converterParaURL(caminhoArquivo);
//         std::cout << "Arquivo selecionado: " << caminhoArquivo << std::endl;

//         return urlArquivo;
//     }

//     void adicionarImg(){
//         std::string link = selecionarImg();
//         if (!link.empty()){
//             img = new Bmp(link.c_str());
//             img->convertBGRtoRGB();
//             if(img->getImage()){
//                 std::cout << "Sucesso ao carregar a imagem!";
//             }else{
//                 std::cout << "Não foi possível carregar a imagem!";
//             }
//         }
//     }



// };


// #endif
