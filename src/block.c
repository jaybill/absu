///
/// J. William McCarthy ("AUTHOR") CONFIDENTIAL
/// Unpublished Copyright (c) 2017-2018 J. William McCarthy, All Rights
/// Reserved.
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

/* block.c */

#include "../include/block.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int block_init(BLOCK *block, int width, int height) {
  block->bufsize = width * height * sizeof(BYTE);
  block->buffer = (char *)malloc(block->bufsize);
  if (block->buffer == NULL) {
    return ERR_CANT_ALLOCATE_MEMORY;
  }
  bzero(block->buffer, block->bufsize);
  block->width = width;
  block->height = height;
  return OK;
}

void block_free(BLOCK *block) {
  free(block->buffer);
  free(block);
}

void block_copy_to_screen(SCREEN *screen, BLOCK *block, int x, int y) {
  // loop through lines of block - copy entire line
  for (size_t i = 0; i < block->height; i++) {
    memcpy(&screen->buffer[x + (screen->width * (y + i))],
           &block->buffer[i * block->width],
           sizeof(block->buffer[0]) * block->width);
  }
}
