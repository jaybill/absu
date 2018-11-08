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
#include <string.h>

#include "../include/types.h"

void fskip(FILE *fp, int num_bytes) {
  int i;
  for (i = 0; i < num_bytes; i++) fgetc(fp);
}

int bitmap_load(char *file, BLOCK *b, bool transparent) {
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

  /* try to allocate memory */
  if (block_init(b, width, height,
                 transparent ? BLOCK_TYPE_RLE : BLOCK_TYPE_NORMAL) != OK) {
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

  for (index = (b->height - 1) * b->width; index >= 0; index -= b->width) {
    for (x = 0; x < b->width; x++) {
      b->buffer[(WORD)index + x] = (BYTE)fgetc(fp);
    }
  }
  fclose(fp);
  if (b->block_type == BLOCK_TYPE_RLE) {
    BYTE *tmp_bitmap = (BYTE *)malloc(b->bufsize);
    memcpy(tmp_bitmap, b->buffer, b->bufsize);

    for (int i = 0; i < b->bufsize; i++) {
      if (i > 0 && i % b->width == 0) printf("\n");
      printf("%02x ", b->buffer[i]);
    }
    bzero(b->buffer, b->bufsize);

    int current_byte = 0;
    int out_bytes = 0;
    int current_row_bytes = 0;
    while (current_byte < b->bufsize) {
      if (tmp_bitmap[current_byte] == 0x00) {
        b->buffer[out_bytes++] = 0x00;
        int still_zero = true;
        int zeroes = 0;
        while (still_zero) {
          if (current_row_bytes < b->width) {
            if (tmp_bitmap[current_byte] == 0x00) {
              zeroes++;
              current_byte++;
              current_row_bytes++;
            } else {
              still_zero = false;
            }
          } else {
            current_row_bytes = 0;
            still_zero = false;
          }
        }
        // TODO - Will need to account for what happens if there are more than
        // 256 (or bytes) zeroes in a row
        b->buffer[out_bytes++] = zeroes;
      } else {
        b->buffer[out_bytes++] = 0x01;
        int start_byte = current_byte;

        bool still_not_zero = true;
        int filled = 0;
        while (still_not_zero) {
          if (current_row_bytes < b->width) {
            if (tmp_bitmap[current_byte] != 0x00) {
              filled++;
              current_byte++;
              current_row_bytes++;
            } else {
              still_not_zero = false;
            }
          } else {
            still_not_zero = false;
            current_row_bytes = 0;
          }
        }
        b->buffer[out_bytes++] = filled;
        memcpy(&b->buffer[out_bytes++], &tmp_bitmap[start_byte], filled);
        out_bytes = out_bytes + filled - 1;
      }
    }

    printf("\nRLE Encoded:\n");

    int out_count = 0;
    while (out_count < b->bufsize) {
      printf("%02x ", b->buffer[out_count]);
      out_count++;
    }

    printf("\n\n");
    free(tmp_bitmap);
  }
  return OK;
}