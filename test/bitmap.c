/**************************************************************************
 * bitmap.c                                                               *
 * written by David Brackeen                                              *
 * http://www.brackeen.com/home/vga/                                      *
 *                                                                        *
 * Tab stops are set to 2.                                                *
 * This program compiles with DJGPP! (www.delorie.com)                    *
 * To compile in DJGPP: gcc bitmap.c -o bitmap.exe                        *
 *                                                                        *
 * This program will only work on DOS- or Windows-based systems with a    *
 * VGA, SuperVGA, or compatible video adapter.                            *
 *                                                                        *
 * Please feel free to copy this source code.                             *
 *                                                                        *
 * DESCRIPTION: This program demostrates drawing bitmaps, including       *
 * transparent bitmaps.                                                   *
 **************************************************************************/

#include <dos.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/nearptr.h>

#define VIDEO_INT 0x10          /* the BIOS video interrupt. */
#define SET_MODE 0x00           /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE 0x13 /* use to set 256-color mode. */
#define TEXT_MODE 0x03          /* use to set 80x25 text mode. */

#define SCREEN_WIDTH 320  /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT 200 /* height in pixels of mode 0x13 */
#define NUM_COLORS 256    /* number of colors in mode 0x13 */

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

byte *VGA = (byte *)0xA0000; /* this points to video memory. */

typedef struct tagBITMAP /* the structure for a bitmap. */
{
  word width;
  word height;
  byte *data;
} BITMAP;

/**************************************************************************
 *  fskip                                                                 *
 *     Skips bytes in a file.                                             *
 **************************************************************************/

void fskip(FILE *fp, int num_bytes) {
  int i;
  for (i = 0; i < num_bytes; i++) fgetc(fp);
}

/**************************************************************************
 *  set_mode                                                              *
 *     Sets the video mode.                                               *
 **************************************************************************/

void set_mode(byte mode) {
  union REGS regs;

  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int86(VIDEO_INT, &regs, &regs);
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
  if ((fp = fopen(file, "rb")) == NULL) {
    printf("Error opening file %s.\n", file);
    exit(1);
  }

  /* check to see if it is a valid bitmap file */
  if (fgetc(fp) != 'B' || fgetc(fp) != 'M') {
    fclose(fp);
    printf("%s is not a bitmap file.\n", file);
    exit(1);
  }

  /* read in the width and height of the image, and the
     number of colors used; ignore the rest */
  fskip(fp, 16);
  fread(&b->width, sizeof(word), 1, fp);
  fskip(fp, 2);
  fread(&b->height, sizeof(word), 1, fp);
  fskip(fp, 22);
  fread(&num_colors, sizeof(word), 1, fp);
  fskip(fp, 6);

  /* assume we are working with an 8-bit file */
  if (num_colors == 0) num_colors = 256;

  /* try to allocate memory */
  if ((b->data = (byte *)malloc((word)(b->width * b->height))) == NULL) {
    fclose(fp);
    printf("Error allocating memory for file %s.\n", file);
    exit(1);
  }

  /* Ignore the palette information for now.
     See palette.c for code to read the palette info. */
  fskip(fp, num_colors * 4);

  /* read the bitmap */
  for (index = (b->height - 1) * b->width; index >= 0; index -= b->width)
    for (x = 0; x < b->width; x++) b->data[(word)index + x] = (byte)fgetc(fp);

  fclose(fp);
}

/**************************************************************************
 *  draw_bitmap                                                           *
 *    Draws a bitmap.                                                     *
 **************************************************************************/

void draw_bitmap(BITMAP *bmp, int x, int y) {
  int j;
  word screen_offset = (y << 8) + (y << 6) + x;
  word bitmap_offset = 0;

  for (j = 0; j < bmp->height; j++) {
    memset(&VGA[screen_offset], &bmp->data[bitmap_offset], bmp->width);

    bitmap_offset += bmp->width;
    screen_offset += SCREEN_WIDTH;
  }
}

/**************************************************************************
 *  draw_transparent_bitmap                                               *
 *    Draws a transparent bitmap.                                         *
 **************************************************************************/

void draw_transparent_bitmap(BITMAP *bmp, int x, int y) {
  int i, j;
  word screen_offset = (y << 8) + (y << 6);
  word bitmap_offset = 0;
  byte data;

  for (j = 0; j < bmp->height; j++) {
    for (i = 0; i < bmp->width; i++, bitmap_offset++) {
      data = bmp->data[bitmap_offset];
      if (data) VGA[screen_offset + x + i] = data;
    }
    screen_offset += SCREEN_WIDTH;
  }
}

/**************************************************************************
 *  Main                                                                  *
 *    Draws opaque and transparent bitmaps                                *
 **************************************************************************/

void main() {
  int i, x, y;
  BITMAP bmp;

  VGA += __djgpp_conventional_base;

  load_bmp("test.bmp", &bmp); /* open the file */

  set_mode(VGA_256_COLOR_MODE); /* set the video mode. */

  /* draw the background */
  for (i = 0; i < 200; i++) memset(&VGA[320 * i], i, SCREEN_WIDTH);

  delay(1000);

  /* draw a tiled bitmap pattern on the left */
  for (y = 0; y <= SCREEN_HEIGHT - bmp.height; y += bmp.height)
    for (x = 0; x <= (SCREEN_WIDTH) / 2 - bmp.width; x += bmp.width)
      draw_bitmap(&bmp, x, y);

  delay(1000);

  /* draw a tiled transparent bitmap pattern on the right */
  for (y = 0; y <= SCREEN_HEIGHT - bmp.height; y += bmp.height)
    for (x = SCREEN_WIDTH - bmp.width; x >= SCREEN_WIDTH / 2; x -= bmp.width)
      draw_transparent_bitmap(&bmp, x, y);

  delay(1000);

  free(bmp.data); /* free up memory used */

  set_mode(TEXT_MODE); /* set the video mode back to
                          text mode. */

  return;
}
