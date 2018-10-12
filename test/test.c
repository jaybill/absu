
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

/* main.c */

#include "test.h"
#include <conio.h>
#include <stdio.h>

#include "../include/block.h"
#include "../include/draw.h"
#include "../include/loop.h"
#include "../include/video.h"

int x, y, frames;
SCREEN *screen;
BLOCK *square1;

bool update() {
  frames++;
  if (kbhit()) {
    return true;
  }
  if (x < 640) {
    x++;
  } else {
    x = 0;
  }
  if (y < 480) {
    y += 5;
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
  screen = video_open();
  square1 = block_create(8, 8);

  draw_hline(square1, 0, 52);
  draw_hline(square1, 7, 52);
  draw_vline(square1, 0, 52);
  draw_vline(square1, 7, 52);

  int fps = loop_run(&update, &render, 60, 5);
  video_close(screen);
  printf("Exiting, ran at %f FPS. %d frames.\n", fps);

  return 0;
}