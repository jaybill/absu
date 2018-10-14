
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
#include <stdio.h>

#include "../include/bitmap.h"
#include "../include/block.h"
#include "../include/draw.h"
#include "../include/loop.h"
#include "../include/types.h"
#include "../include/video.h"

int x, y, b, frames;
SCREEN *screen;
BLOCK *square1;
BLOCK *square2;

bool update() {
  frames++;
  if (kbhit()) {
    return true;
  }
  if (x + b < screen->width) {
    x++;
  } else {
    x = 0;
  }
  if (y + b < screen->height) {
    y += 1;
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
  int err = video_open(screen, MODE_640x480x8);
  
  if (err != OK) {
    printf(
        "ERROR: Your video card must support VBE 2.0, linear framebuffer mode"
        "\n"
        "and be able to display 640x480 @ 8bpp.");
    return 1;
  }

  b = 80;
  int berr = block_init(square1, b, b);
  if (berr != OK) {
    printf("ERROR: Could not allocate memory for block.");
    return 1;
  }

  draw_filled_rect(square1, 0, 0, b, b, 52);

  // BITMAP bmp;

  // load_bmp("test.bmp", &bmp); /* open the file */

  int fps = loop_run(&update, &render, 60, 5);

  video_close(screen);
  block_free(square1);

  char *buf;
  if ((buf = (char *)malloc(PATH_MAX)) == NULL) {
    printf("could not allocate memory for string");
    return 1;
  }

  if (buf && getcwd(buf, PATH_MAX)) {
    printf("cwd is %s\n", buf);
    free(buf);
  } else {
    printf("Got nothing?");
  }

  return 0;
}