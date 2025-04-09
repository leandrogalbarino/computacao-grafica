//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//  Versao 09/2010
//
//**********************************************************

#include "Bmp.h"
#include <string.h>

Bmp::Bmp(const char *fileName)
{
   width = height = 0;
   data = NULL;
   flip = NONE;
   rotation = ROTATE_0;

   if( fileName != NULL && strlen(fileName) > 0 )
   {
      load(fileName);
   }
   else
   {
      printf("Error: Invalid BMP filename");
   }
}


uchar* Bmp::getImage()
{
  return data;
}

int Bmp::getWidth(void)
{
  return width;
}

void Bmp::setPosition(int x, int y){
   posX = x;
   posY = y;
}


int Bmp::getHeight(void)
{
  return height;
}

void Bmp::convertBGRtoRGB()
{
  unsigned char tmp;
  if( data != NULL )
  {
     for(int y=0; y<height; y++)
     for(int x=0; x<width*3; x+=3)
     {
        int pos = y*bytesPerLine + x;
        tmp = data[pos];
        data[pos] = data[pos+2];
        data[pos+2] = tmp;
     }
  }
}

//OBS - colocando dentro da classe BMP, fica muito mais facil e organizado.
// void Bmp::render()
// {
//   unsigned char tmp;

//   if( data != NULL )
//   {
//      for(int y=0; y<height; y++)
//      for(int x=0; x<width*3; x+=3)  //cada 3 coordenadas x representa 1 pixel na imagem (RGB).
//      {
//         int pos = y*bytesPerLine + x;


//         //Dividir pos 3 pois a funcao CV::color() recebe valores entre [0, 1]
//         CV::color(data[pos] / 255.0, data[pos+1] / 255.0, data[pos+2]/ 255.0);
//         CV::point(x/3 + posX , y + posY);
//       //   CV::circleFill(x/3, y, 3, 3);
//      }
//   }
// }
void Bmp::render() {
  if (!data) return;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int srcX = x;
      int srcY = y;

      // Coordenadas de origem de cor
      int pos = srcY * bytesPerLine + srcX * 3;
      float r = data[pos] / 255.0f;
      float g = data[pos + 1] / 255.0f;
      float b = data[pos + 2] / 255.0f;

      // Primeiro: aplicar rotação
      int drawX = 0, drawY = 0;

      switch (rotation) {
        case ROTATE_0:
          drawX = srcX;
          drawY = srcY;
          break;
        case ROTATE_90:
          drawX = height - 1 - srcY;
          drawY = srcX;
          break;
        case ROTATE_180:
          drawX = width - 1 - srcX;
          drawY = height - 1 - srcY;
          break;
        case ROTATE_270:
          drawX = srcY;
          drawY = width - 1 - srcX;
          break;
      }

      // Depois: aplicar flip sobre coordenadas rotacionadas
      if (flip == FLIP_HORIZONTAL || flip == FLIP_BOTH)
        drawX = (rotation % 180 == 0 ? width : height) - 1 - drawX;

      if (flip == FLIP_VERTICAL || flip == FLIP_BOTH)
        drawY = (rotation % 180 == 0 ? height : width) - 1 - drawY;

      // Aplicar posição na tela
      drawX += posX;
      drawY += posY;

      CV::color(r, g, b);
      CV::point(drawX, drawY);
    }
  }
}

 

void Bmp::renderToFit(float targetWidth, float targetHeight)
{
  if (data != nullptr)
  {
    float scaleX = targetWidth / width;
    float scaleY = targetHeight / height;

    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width * 3; x += 3)
      {
        int pos = y * bytesPerLine + x;
        CV::color(data[pos] / 255.0, data[pos + 1] / 255.0, data[pos + 2] / 255.0);

        float px = (x / 3) * scaleX + posX;
        float py = y * scaleY + posY;
        CV::point(px, py); // pode usar CV::rectFill se quiser dar mais corpo ao pixel
      }
    }
  }
}



void Bmp::load(const char *fileName)
{
  FILE *fp = fopen(fileName, "rb");
  if( fp == NULL )
  {
     printf("\nErro ao abrir arquivo %s para leitura", fileName);
     return;
  }

  printf("\n\nCarregando arquivo %s", fileName);

  //le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
  //o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
  fread(&header.type,      sizeof(unsigned short int), 1, fp);
  fread(&header.size,      sizeof(unsigned int),       1, fp);
  fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
  fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
  fread(&header.offset,    sizeof(unsigned int),       1, fp); //indica inicio do bloco de pixels

  //le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
  fread(&info.size,        sizeof(unsigned int),       1, fp);
  fread(&info.width,       sizeof(int),                1, fp);
  fread(&info.height,      sizeof(int),                1, fp);
  fread(&info.planes,      sizeof(unsigned short int), 1, fp);
  fread(&info.bits,        sizeof(unsigned short int), 1, fp);
  fread(&info.compression, sizeof(unsigned int),       1, fp);
  fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
  fread(&info.xresolution, sizeof(int),                1, fp);
  fread(&info.yresolution, sizeof(int),                1, fp);
  fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
  fread(&info.impcolours,  sizeof(unsigned int),       1, fp);

  width  = info.width;
  height = info.height;
  bits   = info.bits;
  bytesPerLine =(3 * (width + 1) / 4) * 4;
  imagesize    = bytesPerLine*height;
  int delta    = bytesPerLine - (3 * width);

  printf("\nImagem: %dx%d - Bits: %d", width, height, bits);
  printf("\nbytesPerLine: %d", bytesPerLine);
  printf("\nbytesPerLine: %d", width * 3);
  printf("\ndelta: %d", delta);
  printf("\nimagesize: %d %d", imagesize, info.imagesize);

  //realiza diversas verificacoes de erro e compatibilidade
  if( header.type != 19778 )
  {
     printf("\nError: Arquivo BMP invalido");
     getchar();
     exit(0);
  }

  if( width*height*3 != imagesize )
  {
     printf("\nWarning: Arquivo BMP nao tem largura multipla de 4");
     getchar();
  }

  if( info.compression != 0 )
  {
     printf("\nError: Formato BMP comprimido nao suportado");
     getchar();
     return;
  }
  if( bits != 24 )
  {
     printf("\nError: Formato BMP com %d bits/pixel nao suportado", bits);
     getchar();
     return;
  }

  if( info.planes != 1 )
  {
     printf("\nError: Numero de Planes nao suportado: %d", info.planes);
     getchar();
     return;
  }

  data = new unsigned char[imagesize];
  fseek(fp, header.offset, SEEK_SET);
  fread(data, sizeof(unsigned char), imagesize, fp);

  fclose(fp);
}
