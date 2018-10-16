#include "../include/bitmap.h"
#include <stdio.h>
#include <stdlib.h>

#include "../include/types.h"

void fskip(FILE *fp, int num_bytes) {
  int i;
  for (i = 0; i < num_bytes; i++) fgetc(fp);
}

int bitmap_load(char *file, BLOCK *b) {
  FILE *fp;
  long index;
  WORD num_colors, width, height;
  int x;

  /* open the file */
  if ((fp = fopen(file, "rb")) == NULL) {
    return ERR_CANT_OPEN_FILE;
  }

  /* check to see if it is a valid bitmap file */
  if (fgetc(fp) != 'B' || fgetc(fp) != 'M') {
    fclose(fp);
    return ERR_NOT_A_BITMAP;
  }

  /* read in the width and height of the image, and the
     number of colors used; ignore the rest */
  fskip(fp, 16);
  fread(&width, sizeof(WORD), 1, fp);
  fskip(fp, 2);
  fread(&height, sizeof(WORD), 1, fp);
  fskip(fp, 22);
  fread(&num_colors, sizeof(WORD), 1, fp);
  fskip(fp, 6);

  /* assume we are working with an 8-bit file */
  if (num_colors == 0) num_colors = 256;
  printf("BMP size: %d,%d\n", width, height);
  /* try to allocate memory */
  if (block_init(b, width, height) != OK) {
    fclose(fp);
    return ERR_CANT_ALLOCATE_MEMORY;
  }

  /* Ignore the palette information for now.
     See palette.c for code to read the palette info. */
  fskip(fp, num_colors * 4);

  /* read the bitmap */
  for (index = (b->height - 1) * b->width; index >= 0; index -= b->width) {
    for (x = 0; x < b->width; x++) {
      b->buffer[(WORD)index + x] = (BYTE)fgetc(fp);
    }
  }
  fclose(fp);
  return OK;
}