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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool once;

int block_init(BLOCK *block, int width, int height, int block_type) {
  block->block_type = block_type;

  block->bufsize =
      width * height *
      sizeof(BYTE);  // TODO: This should account for RLE, but doesn't.
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
  if (block->block_type == BLOCK_TYPE_RLE) {
    int current_block_byte = 0;
    for (size_t i = 0; i < block->height; i++) {
      if (!once) printf("\nLine: %ld\n", i);

      int current_line_byte = 0;
      int skip_bytes = 0;
      while (current_line_byte < block->width &&
             current_block_byte < block->bufsize) {
        if (block->buffer[current_block_byte] == 0x00) {
          current_block_byte++;
          skip_bytes = block->buffer[current_block_byte];
          current_line_byte = current_line_byte + skip_bytes;
          if (!once) printf("s %02d\t", skip_bytes);
          current_block_byte++;
        } else {
          current_block_byte++;
          int write_bytes = block->buffer[current_block_byte];
          current_block_byte++;
          int start_byte = current_block_byte;
          current_line_byte = current_line_byte + write_bytes;
          current_block_byte = current_block_byte + write_bytes;

          if (!once) printf("w %02d\t", write_bytes);

          memcpy(
              &screen
                   ->buffer[x + skip_bytes + (screen->width * (y + i))],
              &block->buffer[start_byte], write_bytes);
              
          
        }
      }
      if (!once) printf("\n");
    }
    once = true;
  } else {
    // loop through lines of block - copy entire line
    for (size_t i = 0; i < block->height; i++) {
      memcpy(&screen->buffer[x + (screen->width * (y + i))],
             &block->buffer[i * block->width],
             sizeof(block->buffer[0]) * block->width);
    }
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
