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

/** @file block.c */

#include "../include/block.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int block_init(BLOCK *block, int width, int height) {
  block->bufsize = width * height * sizeof(BYTE);
  block->buffer = (BYTE *)malloc(block->bufsize);
  if (block->buffer == NULL) {
    return ERR_CANT_ALLOCATE_MEMORY;
  }
  bzero(block->buffer, block->bufsize);
  block->width = width;
  block->height = height;
  return OK;
}

void block_free(BLOCK *block) { free(block->buffer); }

void block_copy_to_screen(SCREEN *screen, BLOCK *block, int x, int y) {
  // loop through lines of block - copy entire line
  for (size_t i = 0; i < block->height; i++) {
    memcpy(&screen->buffer[x + (screen->width * (y + i))],
           &block->buffer[i * block->width],
           sizeof(block->buffer[0]) * block->width);
  }
}

void block_copy_to_block(BLOCK *src, BLOCK *dst, int src_x, int src_y,
                         int src_w, int src_h, int dst_x, int dst_y) {
  for (size_t i = 0; i < src_h; i++) {
    int src_start = ((i + src_y) * src->width) + src_x;
    int dst_start = ((i + dst_y) * dst->width) + dst_x;    
    memcpy(&dst->buffer[dst_start], &src->buffer[src_start],
           sizeof(BYTE) * src_w);
  }
}
