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

#include <dpmi.h>
#include <go32.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/farptr.h>
#include <sys/movedata.h>

__dpmi_regs regs;

unsigned char *screen_buffer;
unsigned short scr_w, scr_h;

byte *VGA = (byte *)VIDEO_MEMORY_START; /* this points to video memory. */
word screen_width, screen_height;

void video_off() {
  free(screen_buffer);
  video_set_mode(TEXT_MODE);
}

int video_init(int screen_w, int screen_h) {
  scr_w = screen_w;
  scr_h = screen_h;

  screen_buffer = (unsigned char *)malloc(scr_w * scr_h);
  if (screen_buffer == NULL) {
    printf("Not enough memory for double buffer.\n");
    return -1;
  }
  video_draw_rect(0, 0, scr_w, scr_h, 0);
  if (video_set_mode(VGA_256_COLOR_MODE) == -1) {
    return -1;
  }
  video_set_unchained_mode(scr_w, scr_h);
  return 0;
}

int video_set_mode(int mode) {
  memset(&regs, 0, sizeof regs);
  regs.x.ax = mode;
  return __dpmi_int(VIDEO_INT, &regs);
}

/**************************************************************************
 *  set_unchained_mode                                                    *
 *    Resets VGA mode 0x13 to unchained mode to access all 256K of        *
 *    memory.  width may be 320 or 360, height may be 200, 400, 240 or    *
 *    480.  If an invalid width or height is specified, it defaults to    *
 *    320x200                                                             *
 **************************************************************************/

void video_set_unchained_mode(int width, int height) {
  word i;
  dword *ptr = (dword *)VGA;

  /* set mode 13 */
  video_set_mode(VGA_256_COLOR_MODE);

  /* turn off chain-4 mode */
  word_out(SC_INDEX, MEMORY_MODE, 0x06);

  /* set map mask to all 4 planes for screen clearing */
  word_out(SC_INDEX, MAP_MASK, 0xff);

  /* clear all 256K of memory */
  for (i = 0; i < 0x4000; i++) *ptr++ = 0;

  /* turn off long mode */
  word_out(CRTC_INDEX, UNDERLINE_LOCATION, 0x00);

  /* turn on byte mode */
  word_out(CRTC_INDEX, MODE_CONTROL, 0xe3);

  screen_width = 320;
  screen_height = 200;

  if (width == 360) {
    /* turn off write protect */
    word_out(CRTC_INDEX, V_RETRACE_END, 0x2c);

    outp(MISC_OUTPUT, 0xe7);
    word_out(CRTC_INDEX, H_TOTAL, 0x6b);
    word_out(CRTC_INDEX, H_DISPLAY_END, 0x59);
    word_out(CRTC_INDEX, H_BLANK_START, 0x5a);
    word_out(CRTC_INDEX, H_BLANK_END, 0x8e);
    word_out(CRTC_INDEX, H_RETRACE_START, 0x5e);
    word_out(CRTC_INDEX, H_RETRACE_END, 0x8a);
    word_out(CRTC_INDEX, OFFSET, 0x2d);

    /* set vertical retrace back to normal */
    word_out(CRTC_INDEX, V_RETRACE_END, 0x8e);

    screen_width = 360;
  } else {
    outp(MISC_OUTPUT, 0xe3);
  }

  if (height == 240 || height == 480) {
    /* turn off write protect */
    word_out(CRTC_INDEX, V_RETRACE_END, 0x2c);

    word_out(CRTC_INDEX, V_TOTAL, 0x0d);
    word_out(CRTC_INDEX, OVERFLOW, 0x3e);
    word_out(CRTC_INDEX, V_RETRACE_START, 0xea);
    word_out(CRTC_INDEX, V_RETRACE_END, 0xac);
    word_out(CRTC_INDEX, V_DISPLAY_END, 0xdf);
    word_out(CRTC_INDEX, V_BLANK_START, 0xe7);
    word_out(CRTC_INDEX, V_BLANK_END, 0x06);
    screen_height = height;
  }

  if (height == 400 || height == 480) {
    word_out(CRTC_INDEX, MAX_SCAN_LINE, 0x40);
    screen_height = height;
  }
}

// void video_put_pixel(int x, int y, int color) {
//   // screen_buffer[(y << 8) + (y << 6) + x] = color;
//   outp(SC_INDEX, MAP_MASK); /* select plane */
//   outp(SC_DATA, 1 << (x & 3));

//   screen_buffer[(y << 6) + (y << 4) + (x >> 2)] = color;
// }

void video_put_pixel(int x, int y, byte color) {
  dword offset;

  outp(SC_INDEX, MAP_MASK); /* select plane */
  outp(SC_DATA, 1 << (x & 3));

  offset = ((dword)y * screen_width + x) >> 2;

  screen_buffer[(word)offset] = color;
}

void video_draw_rect(int x, int y, int w, int h) {
  for (size_t i = 0; i < h; i++) {
    for (size_t j = 0; j < w; j++) {
      video_put_pixel(x + j, y + i, 0);
    }
  }
}

void video_clear_buffer() { memset(screen_buffer, 0, sizeof(screen_buffer)); }

void video_update_screen() {
  
  // while ((inp(INPUT_STATUS_1) & VRETRACE))
  //   ;
  // while (!(inp(INPUT_STATUS_1) & VRETRACE))
  //   ;
  // video_clear_buffer();
  // /* copy the buffer across to the screen */
  // dosmemput(screen_buffer, scr_w * scr_h, VIDEO_MEMORY_START);
  
}

void video_set_palette_register(unsigned char index, rgb_color *color) {
  outportb(0x3c6, 0xff);
  outportb(0x3c8, index);
  outportb(0x3c9, color->red);
  outportb(0x3c9, color->green);
  outportb(0x3c9, color->blue);
}
