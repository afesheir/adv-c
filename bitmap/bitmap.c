#include <stdio.h>
#include <string.h>

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1276

#pragma pack(1)
struct Header {
  char signature[2];
  int size;
  int reserved;
  int offset;
};

struct DIPHeader {
  int dip_header_size;
  int width;
  int height;
  short int nr_planes;
  short int bitcount;
  int compression;
  int img_size;
  int x_px_pm;
  int y_px_pm;
  int used_colors;
  int important_colors;
};

struct Pixel {
  char b;
  char g;
  char r;
};

struct BMPFile {
  struct Header header;
  struct DIPHeader dheader;
  struct Pixel pixels[MAX_HEIGHT * MAX_WIDTH];
};

enum Color {
  WHITE,
  BLACK,
  RED,
  GREEN,
  BLUE
};

struct BMPFile bmp;

void draw_point(int x, int y, enum Color color) {
  int idx = y * bmp.dheader.width + x;
  switch (color) {
    case WHITE:
      bmp.pixels[idx].r = bmp.pixels[idx].g = bmp.pixels[idx].b = 255;
      break;
    case BLACK:
      bmp.pixels[idx].r = bmp.pixels[idx].g = bmp.pixels[idx].b = 0;
      break;
    case RED:
      bmp.pixels[idx].r = 255;
      bmp.pixels[idx].g = bmp.pixels[idx].b = 0;
      break;
    case BLUE:
      bmp.pixels[idx].b = 255;
      bmp.pixels[idx].r = bmp.pixels[idx].g = 0;
      break;
    case GREEN:
      bmp.pixels[idx].g = 255;
      bmp.pixels[idx].r = bmp.pixels[idx].b = 0;
      break;
  }
}

void set_contrast(float perc) {
  for (int i = 0; i < bmp.dheader.width * bmp.dheader.height; i++) {
    bmp.pixels[i].b = (unsigned char)bmp.pixels[i].b * perc;
    bmp.pixels[i].g = (unsigned char)bmp.pixels[i].g * perc;
    bmp.pixels[i].r = (unsigned char)bmp.pixels[i].r * perc;
  }
}
int main(int argc, char * argv[]) {
  struct Header * headerp = &bmp.header;
  struct DIPHeader * dheaderp = &bmp.dheader;
  struct Pixel * pixelp = bmp.pixels;

  strcpy(headerp->signature, "BM");
  headerp->offset = 54;

  dheaderp->width = 8;
  dheaderp->height = 8;
  dheaderp->img_size = 8 * 8 * 3;
  dheaderp->bitcount = 24;
  dheaderp->dip_header_size = 40;
  headerp->size = sizeof(struct Header) + sizeof(struct DIPHeader) + dheaderp->img_size;

  for(int i = 0; i < 8 * 4; i++) {
    pixelp[i].r = pixelp[i].g = pixelp[i].b = 255;
  }

  for(int i = 0; i < 8 * 4; i++) {
    pixelp[i].r = pixelp[i].g = pixelp[i].b = 255;
  }

  draw_point(0, 0, RED);
  draw_point(7, 7, GREEN);

  FILE* fileptr = fopen("/home/ahmed/Desktop/nature.bmp", "rb+");
  fread(&bmp.header, sizeof(struct Header), 1, fileptr);
  fread(&bmp.dheader, bmp.header.size - sizeof(struct Header), 1, fileptr);

  set_contrast(0.5);
  fseek(fileptr, 0, SEEK_SET);
  fwrite(&bmp, headerp->size, 1, fileptr);
  fclose(fileptr);
  return 0;
}