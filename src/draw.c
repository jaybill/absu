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

/* draw.c */

#include "../include/draw.h"
#include <stddef.h>

void draw_pixel(BLOCK *block, int x, int y, int color) {
  block->buffer[x + (block->width * y)] = color;
}

void draw_hline(BLOCK *block, int y, int color) {
  for (size_t i = 0; i < block->width; i++) {
    draw_pixel(block, i, y, color);
  }
}

void draw_vline(BLOCK *block, int x, int color) {
  for (size_t i = 0; i < block->height; i++) {
    draw_pixel(block, x, i, color);
  }
}