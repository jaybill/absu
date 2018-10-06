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

/* graphics.h */

#ifndef MW_GRAPHICS_H
#define MW_GRAPHICS_H

#include <mgrx.h>
#include <stdbool.h>
#include <stdint.h>

extern GrColor *aap64colors;
#define AAP_BLACK aap64colors[0]
#define AAP_BLUE aap64colors[17]
#define AAP_LIGHTBLUE aap64colors[18]
#define AAP_RED aap64colors[3]
#define AAP_WHITE aap64colors[22]

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct graphics_point {
  int32_t x, y;
};

struct graphics_rect {
  struct graphics_point tl, br;
};

bool graphics_start();
void graphics_stop();
void graphics_create_sprite();
void graphics_draw_sprite(struct graphics_point point);
void graphics_draw_background();
void graphics_render_frame();
GrColor *graphics_allocate_palette();

#endif  // !MW_GRAPHICS_H