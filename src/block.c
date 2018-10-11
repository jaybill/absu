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

/* block.c */

#include "block.h"
#include <stddef.h>
#include <stdio.h>

BLOCK *block_create(int width, int height) {
  BLOCK *block = (SCREEN *)malloc(sizeof(BLOCK));
  block->bufsize = width * height * sizeof(BYTE);
  block->buffer = (char *)malloc(block->bufsize);
  bzero(block->buffer, block->bufsize);
  block->width = width;
  block->height = height;
  return block;
}

void block_destroy(BLOCK *block) {
  free(block->buffer);
  free(block);
}

void block_copy_to_screen(SCREEN *screen, BLOCK *block, int x, int y) {
  int drawn_pixels = 0;
  // loop through lines of block
  for (size_t i = 0; i < block->height; i++) {
    // loop through columns of block
    for (size_t j = 0; j < block->width; j++) {
      printf(
          "Block x: %d, Block y: %d, Screen x: %d, Screen y: %d, Color: %d\n",
          j, i, x, y, block->buffer[j + (block->width * i)]);
      screen->buffer[x + j + (screen->width * (y + i))] =
          block->buffer[j + (block->width * i)];
      drawn_pixels++;
    }
  }
  printf("Drew %d pixels.\n", drawn_pixels);
}
