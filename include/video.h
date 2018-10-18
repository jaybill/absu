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

/**
 * @file video.h
 * @brief Video and screen functions.
 */

#ifndef ABSU_VIDEO_H
#define ABSU_VIDEO_H

#include "types.h"

#define MODE_640x480x8 0x101
#define MODE_800x600x8 0x103
#define MODE_1024x768x8 0x105

/**
 * @brief describes the screen.
 */
typedef struct {
  int width;    /**< Width of the screen in pixels */
  int height;   /**< Height of the screen in pixels */
  int bufsize;  /**< Buffer size in BYTEs */
  char *buffer; /**< Pointer to the screen memory buffer */
} SCREEN;

/**
 * @brief describes a single RGB color.
 */
typedef struct {
  BYTE r; /**< Red BYTE of the color */
  BYTE g; /**< Green BYTE of the color */
  BYTE b; /**< Blue BYTE of the color */
} COLOR;

/**
 * @brief initializes SCREEN to one of the available modes.
 * 
 * Choices are MODE_640x480x8, MODE_800x600x8 and MODE_1024x768x8. This must
 * be called on the SCREEN before you can copy a BLOCK to it with
 * @block_copy_to_screen. Returns @OK if it worked and ERR_CANT_GET_VESA_INFO,
 * ERR_NO_LINEAR_FRAMEBUFFER, ERR_CANT_ALLOCATE_MEMORY, ERR_CANT_SET_MODE or
 * ERR_PHYSICAL_MAP_FAILURE if ot doesn't.
 *
 * NOTE: When you are done with graphics mode, you must call @video_close on the
 * SCREEN pointer to free the buffer memory.
 */
int video_open(SCREEN *screen, int video_mode);
/**
 * @brief Sets video back to text mode and frees the screen buffer memory.
 * Generally called right before you exit your program.
 */
void video_close(SCREEN *screen);
/**
 * @brief Copies whatever is in @buffer to the actual video memory
 */
void video_update_screen(SCREEN *screen);
/**
 * @brief sets palette @entry to @color
 */
void video_set_palette(int entry, COLOR color);
/**
 * @brief zeros memory in screen buffer
 */
void video_clear_buffer(SCREEN *screen);
/** 
 * @brief waits until the next refresh
*/
void video_vsync_wait();

#endif  // !ABSU_VIDEO_H