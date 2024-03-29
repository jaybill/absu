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

/** @file block.h
 * @brief Functions for initializing and operating on a BLOCK
 */

#ifndef ABSU_BLOCK_H
#define ABSU_BLOCK_H

#include "video.h"

typedef struct {
  int width, height, bufsize;
  BYTE *buffer;
  int block_type;
} BLOCK;

#define BLOCK_TYPE_NORMAL 1
#define BLOCK_TYPE_RLE 2

int block_init(BLOCK *block, int width, int height, int block_type);
void block_free(BLOCK *block);
void block_copy_to_screen(SCREEN *screen, BLOCK *block, int x, int y);
int block_copy_to_block(BLOCK *src, BLOCK *dst, int src_x, int src_y,
                         int src_w, int src_h, int dst_x, int dst_y);

#endif  // !ABSU_BLOCK_H