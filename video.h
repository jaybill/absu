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

#include <stdbool.h>

#define SET_MODE 0x10  /* BIOS func to set the video mode. */
#define VGA_HIGH 0x12  /* use to set 640x680 256-color mode. */
#define VGA_LOW 0x13   /* use to set 320x200 256-color mode. */
#define TEXT_MODE 0x03 /* use to set 80x25 text mode. */

#define SCREEN_WIDTH_HI 640  /* width in pixels of mode 0x12 */
#define SCREEN_HEIGHT_HI 480 /* height in pixels of mode 0x12 */
#define SCREEN_WIDTH_LO 320  /* width in pixels of mode 0x12 */
#define SCREEN_HEIGHT_LO 200 /* height in pixels of mode 0x12 */

#define VIDEO_MEMORY_START 0xa0000

#define NUM_COLORS 256 /* number of colors in mode 0x12 and 0x13 */

int video_set_mode(int mode);
void video_draw_pixel(int x, int y, int c);
int video_get_vesa_info();
int video_get_mode_info(int mode);
int video_find_vesa_mode(int w, int h);
int video_set_vesa_mode(int w, int h);
void video_putpixel_vesa_640x480(int x, int y, int color);
void video_set_vesa_bank(int bank_number);

#endif  // !MW_VIDEO_H