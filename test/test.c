
/// Copyright (C) 2018, John William McCarthy
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
#include <stdio.h>

#include "../include/block.h"
#include "../include/draw.h"
#include "../include/video.h"

int main(void) {
  SCREEN *screen = video_open();

  BLOCK *square1 = block_create(20, 8);

  draw_hline(square1, 0, 52);
  draw_hline(square1, 7, 52);
  draw_vline(square1, 0, 52);
  draw_vline(square1, 19, 52);

  block_copy_to_screen(screen, square1, 10, 10);

  video_update_screen(screen);

  getkey();

  video_close(screen);
  return 0;
}