
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

/* test.c */

#include <conio.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/bitmap.h"
#include "../include/block.h"
#include "../include/draw.h"
#include "../include/loop.h"
#include "../include/types.h"
#include "../include/video.h"

int x, y, b, frames;
SCREEN *screen;
BLOCK *square1;

bool update() {
  frames++;
  if (kbhit()) {
    return true;
  }
  if (x + square1->width < screen->width) {
    x++;
  } else {
    x = 0;
  }
  if (y + square1->height < screen->height) {
    y++;
  } else {
    y = 0;
  }

  return false;
}

void render() {
  video_clear_buffer(screen);
  block_copy_to_screen(screen, square1, x, y);
  video_update_screen(screen);
}

int main(void) {
  if ((screen = (SCREEN *)malloc(sizeof(SCREEN))) == NULL) {
    printf("ERROR: Could not allocate memory for screen.");
    return 1;
  }

  int err = video_open(screen, MODE_640x480x8);

  if (err != OK) {
    printf(
        "ERROR: Your video card must be VESA 2.0, have linear framebuffer \n"
        "mode and be able to display 640x480 @ 8bpp.");
    return 1;
  }

  if ((square1 = (BLOCK *)malloc(sizeof(BLOCK))) == NULL) {
    printf("ERROR: Couldn't allocate memory for bitmap.");
    return 1;
  }

  if (bitmap_load("test.bmp", square1) != OK) {
    printf("ERROR: Can't load bitmap.");
  }

  loop_run(&update, &render, 60, 5);
  video_close(screen);

  block_free(square1);
  printf("Exiting sanely.\n");
  return 0;
}