
/*
 * Copyright (C) 2018, Jaybill McCarthy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/* test.c */

#include <conio.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/bitmap.h"
#include "../include/block.h"
#include "../include/draw.h"
#include "../include/keyboard.h"
#include "../include/loop.h"
#include "../include/types.h"
#include "../include/video.h"

#define SPEED 1

typedef struct {
  BLOCK *sprite;
  int x, y;
  int x_direction, y_direction;
} PLAYER;

SCREEN *screen;
PLAYER *player;
int k;

bool update() {
  bool exit_now = false;

  if (keyboard_key_down(KEY_UP)) {
    player->y_direction = -SPEED;
    player->x_direction = 0;
  } else if (keyboard_key_down(KEY_DOWN)) {
    player->y_direction = SPEED;
    player->x_direction = 0;
  } else if (keyboard_key_down(KEY_LEFT)) {
    player->x_direction = -SPEED;
    player->y_direction = 0;
  } else if (keyboard_key_down(KEY_RIGHT)) {
    player->x_direction = SPEED;
    player->y_direction = 0;
  } else {
    player->x_direction = 0;
    player->y_direction = 0;
  }

  if (keyboard_key_down(KEY_ESC)) {
    exit_now = true;
  }

  // move
  player->x += player->x_direction;
  player->y += player->y_direction;

  // check for bouncing
  if (player->x < 0) {
    player->x = 0;
    player->x_direction = -player->x_direction;  // move in other direction
  } else {
    if (player->x > screen->width - player->sprite->width) {
      player->x = screen->width - player->sprite->width;
      player->x_direction = -player->x_direction;  // move in other direction
    }
  }
  if (player->y < 0) {
    player->y = 0;
    player->y_direction = -player->y_direction;  // move in other direction
  } else {
    if (player->y > screen->height - player->sprite->height) {
      player->y = screen->height - player->sprite->height;
      player->y_direction = -player->y_direction;  // move in other direction
    }
  }

  return exit_now;
}

void render() {
  video_clear_buffer(screen);
  block_copy_to_screen(screen, player->sprite, player->x, player->y);
  video_update_screen(screen);
}

int main(void) {
  SCREEN s;
  screen = &s;

  int err = video_open(screen, MODE_640x480x8);

  if (err != OK) {
    printf(
        "ERROR: Your video card must be VESA 2.0, have linear framebuffer \n"
        "mode and be able to display 640x480 @ 8bpp.");
    return 1;
  }

  if (keyboard_init() != OK) {
    printf("ERROR: Could not install keyboard handler.\n");
    return 1;
  }

  keyboard_chain(OFF);

  BLOCK b;
  PLAYER p;
  player = &p;
  player->sprite = &b;

  if (bitmap_load("test.bmp", player->sprite) != OK) {
    printf("ERROR: Can't load test.bmp");
  }

  loop_run(&update, &render, 60);
  video_close(screen);

  block_free(player->sprite);

  keyboard_chain(ON);
  keyboard_close();

  printf("Exiting sanely.\n");
  return 0;
}