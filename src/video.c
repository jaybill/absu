///
/// J. William McCarthy ("AUTHOR") CONFIDENTIAL
/// Unpublished Copyright (c) 2017-2018 J. William McCarthy, All Rights
/// Reserved.
///
/// NOTICE:  All information contained herein is, and remains the property of
/// AUTHOR. The intellectual and technical concepts contained herein are
/// proprietary to AUTHOR and may be covered by U.S. and Foreign Patents,
/// patents in process, and are protected by trade secret or copyright law.
/// Dissemination of this information or reproduction of this material is
/// strictly forbidden unless prior written permission is obtained from AUTHOR.
/// Access to the source code contained herein is hereby forbidden to anyone
/// except current AUTHOR employees, managers or contractors who have executed
/// Confidentiality and Non-disclosure agreements explicitly covering such
/// access.
///
/// The copyright notice above does not evidence any actual or intended
/// publication or disclosure  of  this source code, which includes information
/// that is confidential and/or proprietary, and is a trade secret, of  AUTHOR.
/// ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC  PERFORMANCE, OR PUBLIC
/// DISPLAY OF OR THROUGH USE  OF THIS  SOURCE CODE  WITHOUT  THE EXPRESS
/// WRITTEN CONSENT OF AUTHOR IS STRICTLY PROHIBITED, AND IN VIOLATION OF
/// APPLICABLE LAWS AND INTERNATIONAL TREATIES.  THE RECEIPT OR POSSESSION OF
/// THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY
/// RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE,
/// USE, OR SELL ANYTHING THAT IT  MAY DESCRIBE, IN WHOLE OR IN PART.
///

/* video.c */
#include "video.h"

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

void video_update_screen(SCREEN *screen) {
  memcpy(video, screen->buffer, screen->bufsize);
}

void video_draw_test() {}