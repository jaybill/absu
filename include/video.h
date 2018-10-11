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

/* video.h */

#ifndef ABSU_VIDEO_H
#define ABSU_VIDEO_H

#define BYTE unsigned char

typedef struct screens {
  int width, height, bpp, bufsize;
  char *buffer;
} SCREEN;

SCREEN *video_open();
void video_close(SCREEN *screen);
void video_update_screen(SCREEN *screen);
void video_set_palette(int entry, int r, int g, int b);
void video_clear_buffer(SCREEN *screen);

#endif  // !ABSU_VIDEO_H