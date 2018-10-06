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

/* graphics.c */

#include "graphics.h"
#include <stddef.h>
#include <stdio.h>

GrContext *sprite;
GrContext *screen_buffer;
GrContext *bg;
GrContext *title;
GrColor *aap64colors;

static struct {
  long color;
} AAP64[64] = {
    {0x060608},  //	0 - BLACK
    {0x141013},  //	1
    {0x3b1725},  //	2
    {0x73172d},  //	3 - RED
    {0xb4202a},  //	4
    {0xdf3e23},  //	5
    {0xfa6a0a},  //	6
    {0xf9a31b},  //	7
    {0xffd541},  //	8
    {0xfffc40},  //	9
    {0xd6f264},  //	10
    {0x9cdb43},  //	11
    {0x59c135},  //	12
    {0x14a02e},  //	13
    {0x1a7a3e},  //	14
    {0x24523b},  //	15
    {0x122020},  //	16
    {0x143464},  //	17 - BLUE
    {0x285cc4},  //	18 - LIGHTBLUE
    {0x249fde},  //	19
    {0x20d6c7},  //	20
    {0xa6fcdb},  //	21
    {0xffffff},  //	22 - WHITE
    {0xfef3c0},  //	23
    {0xfad6b8},  //	24
    {0xf5a097},  //	25
    {0xe86a73},  //	26
    {0xbc4a9b},  //	27
    {0x793a80},  //	28
    {0x403353},  //	29
    {0x242234},  //	30
    {0x221c1a},  //	31
    {0x322b28},  //	32
    {0x71413b},  //	33
    {0xbb7547},  //	34
    {0xdba463},  //	35
    {0xf4d29c},  //	36
    {0xdae0ea},  //	37
    {0xb3b9d1},  //	38
    {0x8b93af},  //	39
    {0x6d758d},  //	40
    {0x4a5462},  //	41
    {0x333941},  //	42
    {0x422433},  //	43
    {0x5b3138},  //	44
    {0x8e5252},  //	45
    {0xba756a},  //	46
    {0xe9b5a3},  //	47
    {0xe3e6ff},  //	48
    {0xb9bffb},  //	49
    {0x849be4},  //	50
    {0x588dbe},  //	51
    {0x477d85},  //	52
    {0x23674e},  //	53
    {0x328464},  //	54
    {0x5daf8d},  //	55
    {0x92dcba},  //	56
    {0xcdf7e2},  //	57
    {0xe4d2aa},  //	58
    {0xc7b08b},  //	59
    {0xa08662},  //	60
    {0x796755},  //	61
    {0x5a4e44},  //	62
    {0x423934}   //	63
};

GrColor *graphics_allocate_palette(void) {
  static GrColor pal[64];
  int i;
  for (i = 0; i < 64; ++i) {
    pal[i] = GrAllocColor2(AAP64[i].color);
  }
  return pal;
}

bool graphics_start() {
  if (!GrSetMode(GR_width_height_bpp_graphics, SCREEN_WIDTH, SCREEN_HEIGHT,
                 8)) {
    return false;
  }
  aap64colors = graphics_allocate_palette();
  screen_buffer = GrCreateContext(SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL);
  GrSetContext(screen_buffer);
  return true;
}

void graphics_stop() {
  GrSetContext(GrScreenContext());
  GrDestroyContext(screen_buffer);
  GrDestroyContext(sprite);
  GrSetMode(GR_TEXT_DEFAULT);
}

void graphics_create_sprite() {
  int w, h;
  char *filename = "test.png";
  GrQueryPng(filename, &w, &h);
  sprite = GrCreateContext(16, 16, NULL, NULL);
  GrSetContext(sprite);
  GrLoadContextFromPng(sprite, filename, true);
  GrSaveContext(sprite);

  bg = GrCreateContext(SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL);
  GrSetContext(bg);
  GrFilledBox(0, 0, bg->gc_xmax, bg->gc_ymax, AAP_BLUE);
  GrSaveContext(bg);

  title = GrCreateContext(SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL);
  GrSetContext(title);
  GrFilledBox(0, 0, title->gc_xmax, title->gc_ymax, AAP_BLUE);
  char *message = "Molewarp";
  int x, y;
  GrTextOption grt;
  grt.txo_font = GrGetDefaultFont();
  grt.txo_fgcolor = AAP_WHITE;
  grt.txo_bgcolor = AAP_BLACK;
  grt.txo_direct = GR_TEXT_RIGHT;
  grt.txo_xalign = GR_ALIGN_CENTER;
  grt.txo_yalign = GR_ALIGN_CENTER;
  grt.txo_chrtype = GR_BYTE_TEXT;

  GrBox(0, 0, GrMaxX(), GrMaxY(), AAP_WHITE);
  GrBox(4, 4, GrMaxX() - 4, GrMaxY() - 4, AAP_WHITE);

  x = GrMaxX() / 2;
  y = GrMaxY() / 2;

  GrDrawString(message, strlen(message), x, y, &grt);

  GrSaveContext(title);

  GrSetContext(GrScreenContext());
}

void graphics_draw_background() {
  GrBitBlt(screen_buffer, 0, 0, bg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GrWRITE);
}

void graphics_draw_title() {
  GrBitBlt(screen_buffer, 0, 0, title, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GrWRITE);
}

void graphics_draw_sprite(struct graphics_point point) {
  GrBitBlt(screen_buffer, point.x, point.y, sprite, 0, 0, sprite->gc_xmax,
           sprite->gc_ymax, GrWRITE);
}

void graphics_render_frame() {
  GrBitBlt(GrScreenContext(), 0, 0, screen_buffer, 0, 0, SCREEN_WIDTH,
           SCREEN_HEIGHT, GrWRITE);
}