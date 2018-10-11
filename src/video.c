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

/* video.c */
#include "../include/video.h"

#include <dos.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <stdio.h>
#include <stdlib.h>

#include <crt0.h>
#include <sys/nearptr.h>

int _crt0_startup_flags = _CRT0_FLAG_NEARPTR | _CRT0_FLAG_NONMOVE_SBRK;

typedef struct {
  unsigned ModeAttributes;
  unsigned granularity, startseg, farfunc;
  short bscanline;
  short XResolution;
  short YResolution;
  short charpixels;
  unsigned bogus1, bogus2, bogus3, bogus4;
  unsigned PhysBasePtr;
  char bogus[228];
} ModeInfoBlock;

unsigned int ADDR;
int width, height, bufsize;
char *video;

ModeInfoBlock *video_get_mode_info(int mode) {
  static ModeInfoBlock info;
  __dpmi_regs r;

  /* Use the transfer buffer to store the results of VBE call */
  r.x.ax = 0x4F01;
  r.x.cx = mode;
  r.x.es = __tb / 16;
  r.x.di = 0;
  __dpmi_int(0x10, &r);
  if (r.h.ah) return 0;
  dosmemget(__tb, sizeof(ModeInfoBlock), &info);
  return &info;
}

void video_set_palette(int entry, int r, int g, int b) {
  outportb(0x3c8, (BYTE)entry);
  outportb(0x3c9, (BYTE)r);
  outportb(0x3c9, (BYTE)g);
  outportb(0x3c9, (BYTE)b);
}

SCREEN *video_open() {
  __dpmi_meminfo info;
  __dpmi_regs reg;
  ModeInfoBlock *mb;

  mb = video_get_mode_info(0x101);

  if (!mb) {
    printf("Get VESA mode info failed.\n");
    exit(1);
  }
  if (!(mb->ModeAttributes & 0x80)) {
    printf("Linear frame buffer not supported for VESA mode.\n");
    exit(1);
  }

  width = mb->XResolution;
  height = mb->YResolution;
  info.size = width * height;
  info.address = mb->PhysBasePtr;
  if (__dpmi_physical_address_mapping(&info) == -1) {
    printf("Physical mapping of address 0x%x failed!\n", mb->PhysBasePtr);
    exit(2);
  }
  ADDR = info.address; /* Updated by above call */
  printf("Vesa mode 0x101: %d x %d, linear frame: 0x%x\n", width, height, ADDR);

  reg.x.ax = 0x4f02;
  reg.x.bx = 0x4101;      /* mode plus linear enable bit */
  __dpmi_int(0x10, &reg); /* set the mode */
  if (reg.h.al != 0x4f || reg.h.ah) {
    printf("Mode set failed!\n");
    exit(3);
  }

  video = (char *)(ADDR + __djgpp_conventional_base);
  bufsize = width * height * sizeof(BYTE);

  SCREEN *screen = (SCREEN *)malloc(sizeof(SCREEN));
  screen->buffer = (char *)malloc(bufsize);
  screen->bufsize = bufsize;
  screen->width = width;
  screen->height = height;

  return screen;
}

void video_close(SCREEN *screen) {
  __dpmi_regs reg;
  reg.x.ax = 0x0003;
  __dpmi_int(0x10, &reg);
  free(screen->buffer);
  free(screen);
}

void video_clear_buffer(SCREEN *screen) {
  bzero(screen->buffer,screen->bufsize);
}

void video_clear_screen(){
  bzero(video,bufsize);
}

void video_update_screen(SCREEN *screen) {
  memcpy(video, screen->buffer, screen->bufsize);
}