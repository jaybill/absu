///
/// Copyright (C) 2018, Jaybill McCarthy
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///

/* bitmap.c */

#include "../include/bitmap.h"

#include <dos.h>
#include <dpmi.h>
#include <go32.h>
#include <limits.h>
#include <pc.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/nearptr.h>
#include <unistd.h>
#include "../include/bitmap.h"

int bitmap_load(char *file) { return OK; }

void fskip(FILE *fp, int num_bytes) {
  int i;
  for (i = 0; i < num_bytes; i++) fgetc(fp);
}

/**************************************************************************
 *  load_bmp                                                              *
 *    Loads a bitmap file into memory.                                    *
 **************************************************************************/

void load_bmp(char *file, BITMAP *b) {
  FILE *fp;
  long index;
  word num_colors;
  int x;

  /* open the file */

  fp = fopen(file, "rb");

  // // printf("FP is %d\n",fp->_bufsiz);
  // return;
  // if (fp) {
  //   printf("Error opening file %s.\n", file);
  //   exit(1);
  // }

  // /* check to see if it is a valid bitmap file */
  // if (fgetc(fp) != 'B' || fgetc(fp) != 'M') {
  //   fclose(fp);
  //   printf("%s is not a bitmap file.\n", file);
  //   exit(1);
  // }
  // return;
  // /* read in the width and height of the image, and the
  //    number of colors used; ignore the rest */
  // fskip(fp, 16);
  // fread(&b->width, sizeof(word), 1, fp);
  // fskip(fp, 2);
  // fread(&b->height, sizeof(word), 1, fp);
  // fskip(fp, 22);
  // fread(&num_colors, sizeof(word), 1, fp);
  // fskip(fp, 6);

  // /* assume we are working with an 8-bit file */
  // if (num_colors == 0) num_colors = 256;

  // /* try to allocate memory */
  // if ((b->data = (byte *)malloc((word)(b->width * b->height))) == NULL) {
  //   fclose(fp);
  //   printf("Error allocating memory for file %s.\n", file);
  //   exit(1);
  // }

  // /* Ignore the palette information for now.
  //    See palette.c for code to read the palette info. */
  // fskip(fp, num_colors * 4);

  // /* read the bitmap */
  // for (index = (b->height - 1) * b->width; index >= 0; index -= b->width)
  //   for (x = 0; x < b->width; x++) b->data[(word)index + x] =
  //   (byte)fgetc(fp);

  fclose(fp);
}

/**************************************************************************
 *  draw_bitmap                                                           *
 *    Draws a bitmap.                                                     *
 **************************************************************************/

// void draw_bitmap(BITMAP *bmp, int x, int y) {
//   int j;
//   word screen_offset = (y << 8) + (y << 6) + x;
//   word bitmap_offset = 0;

//   for (j = 0; j < bmp->height; j++) {
//     memset(&VGA[screen_offset], &bmp->data[bitmap_offset], bmp->width);

//     bitmap_offset += bmp->width;
//     screen_offset += SCREEN_WIDTH;
//   }
// }

/**************************************************************************
 *  draw_transparent_bitmap                                               *
 *    Draws a transparent bitmap.                                         *
 **************************************************************************/

// void draw_transparent_bitmap(BITMAP *bmp, int x, int y) {
//   int i, j;
//   word screen_offset = (y << 8) + (y << 6);
//   word bitmap_offset = 0;
//   byte data;

//   for (j = 0; j < bmp->height; j++) {
//     for (i = 0; i < bmp->width; i++, bitmap_offset++) {
//       data = bmp->data[bitmap_offset];
//       if (data) VGA[screen_offset + x + i] = data;
//     }
//     screen_offset += SCREEN_WIDTH;
//   }
// }