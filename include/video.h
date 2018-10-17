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

/* video.h */

/** 
 * @file video.h
 * @brief A brief file description.
 * A more elaborated file description.
 */

#ifndef ABSU_VIDEO_H
#define ABSU_VIDEO_H

#include "types.h"

#define MODE_640x480x8 0x101
#define MODE_800x600x8 0x103
#define MODE_1024x768x8 0x105

/**
 * A global integer value.
 * More details about this value.
 */
typedef struct screens {
  int width; /**< Width of the screen in pixels */
  int height;
  int bufsize;
  char *buffer;
} SCREEN;

typedef struct colors {
  BYTE r, g, b;
} COLOR;

SCREEN *video_new_screen();
int video_open(SCREEN *screen, int video_mode);
void video_close(SCREEN *screen);
void video_update_screen(SCREEN *screen);
void video_set_palette(int entry, COLOR color);
void video_clear_buffer(SCREEN *screen);
void video_vsync_wait();

#endif  // !ABSU_VIDEO_H