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

/** @file bitmap.h
 * @brief Functions for interacting with bitmap files
 */

#ifndef ABSU_BITMAP_H
#define ABSU_BITMAP_H

#include "block.h"
#include "types.h"

/**
 * Takes a filename and pointer to a block. Loads the bitmap into the
 * block and sets the height, width and bufsize properties of the block
 * accordingly. Returns @OK if it worked and ERR_CANT_OPEN_FILE,
 * ERR_NOT_A_BITMAP or ERR_CANT_ALLOCATE_MEMORY if ot doesn't.
 *
 * NOTE: If you create a BLOCK with this, you must call @block_free on it when
 * you're done to free the buffer memory.
 */
int bitmap_load(char *file, BLOCK *b);

#endif  // !ABSU_BITMAP_H