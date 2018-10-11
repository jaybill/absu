
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

/* main.c */

#include "main.h"
#include "block.h"
#include "draw.h"
#include "video.h"

void main(void) {
  SCREEN *screen = video_open();

  BLOCK *square1 = block_create(20, 20);

  draw_hline(square1, 0, 52);
  draw_hline(square1, 19, 52);
  draw_vline(square1, 0, 52);
  draw_vline(square1, 19, 52);

  block_copy_to_screen(screen, square1, -5, 200);

  video_update_screen(screen);

  getkey();
  video_close(screen);
}