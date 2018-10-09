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

/* video.h */

#ifndef MW_VIDEO_H
#define MW_VIDEO_H

#define VIDEO_MEMORY_START 0xa0000
#define INPUT_STATUS_1 0x03da
#define VRETRACE 0x08
#define VIDEO_INT 0x10          /* the BIOS video interrupt. */
#define SET_MODE 0x00           /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE 0x13 /* use to set 256-color mode. */
#define TEXT_MODE 0x03          /* use to set 80x25 text mode. */
#define MISC_OUTPUT 0x03c2      /* VGA misc. output register */
#define SC_INDEX 0x03c4         /* VGA sequence controller */
#define SC_DATA 0x03c5
#define PALETTE_INDEX 0x03c8 /* VGA digital-to-analog converter */
#define PALETTE_DATA 0x03c9
#define CRTC_INDEX 0x03d4 /* VGA CRT controller */
#define MAP_MASK 0x02     /* Sequence controller registers */
#define MEMORY_MODE 0x04
#define H_TOTAL 0x00 /* CRT controller registers */
#define H_DISPLAY_END 0x01
#define H_BLANK_START 0x02
#define H_BLANK_END 0x03
#define H_RETRACE_START 0x04
#define H_RETRACE_END 0x05
#define V_TOTAL 0x06
#define OVERFLOW 0x07
#define MAX_SCAN_LINE 0x09
#define V_RETRACE_START 0x10
#define V_RETRACE_END 0x11
#define V_DISPLAY_END 0x12
#define OFFSET 0x13
#define UNDERLINE_LOCATION 0x14
#define V_BLANK_START 0x15
#define V_BLANK_END 0x16
#define MODE_CONTROL 0x17

#define NUM_COLORS 256 /* number of colors in mode 0x13 */

/* macro to write a word to a port */
#define word_out(port, register, value) \
  outpw(port, (((word)value << 8) + register))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

typedef struct rgb_color_type {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} rgb_color;

int video_set_mode(int mode);
void video_put_pixel(int x, int y, byte color);
int video_init(int screen_w, int screen_h);
void video_set_unchained_mode(int width, int height);
void video_set_palette_register(unsigned char index, rgb_color *color);
void video_update_screen();
void video_off();
void video_clear_buffer();
void video_draw_line(int x1, int y1, int x2, int y2);

#endif  // !MW_VIDEO_H