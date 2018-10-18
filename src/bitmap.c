/*
 * Copyright (C) 2018, Jaybill McCarthy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/** @file bitmap.c */

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

  // open the file
  if ((fp = fopen(file, "rb")) == NULL) {
    return ERR_CANT_OPEN_FILE;
  }

  // check to see if it is a valid bitmap file
  if (fgetc(fp) != 'B' || fgetc(fp) != 'M') {
    fclose(fp);
    return ERR_NOT_A_BITMAP;
  }

  // read in the width and height of the image, and the number of colors used;
  // ignore the rest
  fskip(fp, 16);
  fread(&width, sizeof(WORD), 1, fp);
  fskip(fp, 2);
  fread(&height, sizeof(WORD), 1, fp);
  fskip(fp, 22);
  fread(&num_colors, sizeof(WORD), 1, fp);
  fskip(fp, 6);

  // assume we are working with an 8-bit file
  if (num_colors == 0) num_colors = 256;
  printf("BMP size: %d,%d\n", width, height);
  /* try to allocate memory */
  if (block_init(b, width, height) != OK) {
    fclose(fp);
    return ERR_CANT_ALLOCATE_MEMORY;
  }

  // read the palette information
  // TODO: Set this once outside of this function, not here
  int p = 0;
  for (index = 0; index < num_colors; index++) {
    COLOR c;
    c.b = fgetc(fp) >> 2;
    c.g = fgetc(fp) >> 2;
    c.r = fgetc(fp) >> 2;
    video_set_palette(index, c);
    x = fgetc(fp);
    p++;
  }

  // read the bitmap
  for (index = (b->height - 1) * b->width; index >= 0; index -= b->width) {
    for (x = 0; x < b->width; x++) {
      b->buffer[(WORD)index + x] = (BYTE)fgetc(fp);
    }
  }
  fclose(fp);

  return OK;
}