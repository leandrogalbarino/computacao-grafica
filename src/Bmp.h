
#ifndef ___BMP__H___
#define ___BMP__H___

#include <stdio.h>
#include <stdlib.h>
#include "Flip.h"

#include "gl_canvas2d.h"

#define HEADER_SIZE 14     // sizeof(HEADER) vai dar 16 devido ao alinhamento de bytes
#define INFOHEADER_SIZE 40 // sizeof(INFOHEADER) da 40 e esta correto.
#define uchar unsigned char

typedef struct
{
   unsigned short int type; /* Magic identifier            */
   unsigned int size;       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset; /* Offset to image data em bytes*/
} HEADER;

typedef struct
{
   unsigned int size;            /* Header size in bytes      */
   int width, height;            /* Width and height of image */
   unsigned short int planes;    /* Number of colour planes   */
   unsigned short int bits;      /* Bits per pixel            */
   unsigned int compression;     /* Compression type          */
   unsigned int imagesize;       /* Image size in bytes       */
   int xresolution, yresolution; /* Pixels per meter          */
   unsigned int ncolours;        /* Number of colours         */
   unsigned int impcolours;      /* Important colours         */
} INFOHEADER;

class Bmp
{
private:
   int width, height, imagesize, bytesPerLine, bits;
   unsigned char *data;

   HEADER header;
   INFOHEADER info;

   void load(const char *fileName);
   FlipType flip;
   float brightness = 1.0f; // padrão

public:
   int posX, posY;
   Bmp(const char *fileName);
   void setPosition(int x, int y);
   uchar *getImage();
   int getWidth(void);
   int getHeight(void);
   void convertBGRtoRGB(void);
   void render();
   void renderToFit(float targetWidth, float targetHeight);
   void setFlip(FlipType f) { flip = f; }
   FlipType getFlip() { return flip; }
   float getBrightnesse() { return brightness; }
   void setBrightness(float value) { brightness = value; }
};

#endif
