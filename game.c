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

/* game.c */

#include "game.h"

#include <conio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "graphics.h"

// Define Constants
#define COLOR_BACKGROUND AAP_BLACK
#define COLOR_PLAYER AAP_BLUE
#define SPEED 10
#define SQUARE_SIZE 16
#define TARGET_FPS 120
#define MAX_FRAMESKIP 5

int x_max = SCREEN_WIDTH - SQUARE_SIZE;
int y_max = SCREEN_HEIGHT - SQUARE_SIZE;
static bool exit_now = false;

struct movement {
  double x_direction, y_direction, speed;
};

struct player {
  struct graphics_point location;
  struct movement movement;
};

struct player player;

static void process_input() {}

static void player_draw() {
  // Draw the sprite
  graphics_draw_sprite(player.location);
}

void render() {
  graphics_draw_background();
  player_draw();
  graphics_render_frame();
}

static void player_step() {
  // Get input
  if (kbhit()) {
    int key = getch();
    if (key > 0) {
      switch (key) {
        case 'w':
          player.movement.y_direction = -SPEED;
          break;
        case 's':
          player.movement.y_direction = SPEED;
          break;
        case 'a':
          player.movement.x_direction = -SPEED;
          break;
        case 'd':
          player.movement.x_direction = SPEED;
          break;
        case 27:
          exit_now = true;
          break;
      }
    }
  } else {
    player.movement.x_direction = 0;
    player.movement.y_direction = 0;
  }

  // move
  player.location.x += player.movement.x_direction;
  player.location.y += player.movement.y_direction;

  // check for bouncing
  if (player.location.x < 0) {
    player.location.x = 0;
    player.movement.x_direction =
        -player.movement.x_direction;  // move in other direction
  } else {
    if (player.location.x > x_max) {
      player.location.x = x_max;
      player.movement.x_direction =
          -player.movement.x_direction;  // move in other direction
    }
  }
  if (player.location.y < 0) {
    player.location.y = 0;
    player.movement.y_direction =
        -player.movement.y_direction;  // move in other direction
  } else {
    if (player.location.y > y_max) {
      player.location.y = y_max;
      player.movement.y_direction =
          -player.movement.y_direction;  // move in other direction
    }
  }
}

void update() { player_step(); }

void game(void) {
  graphics_create_sprite();

  int key = 0;
  player.movement.x_direction = SPEED;
  player.movement.y_direction = SPEED;
  int32_t skip_ticks = UCLOCKS_PER_SEC / TARGET_FPS;
  uclock_t next_game_tick = uclock();
  uclock_t start_time = next_game_tick;
  int loops;
  float interpolation;
  uclock_t game_time = uclock();
  while (!exit_now) {
    game_time = uclock();
    loops = 0;

    while (game_time > next_game_tick && loops < MAX_FRAMESKIP) {
      process_input();
      update();
      next_game_tick += skip_ticks;
      loops++;
    }

    render();
  }
}