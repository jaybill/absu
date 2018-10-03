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
#define PLAYER_MOVEMENT_SPEED 2
#define SQUARE_SIZE 16
#define TARGET_FPS 60
#define MAX_FRAMESKIP 5

int renders = 0;
int updates = 0;
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

static void player_draw(float delta) {
  // view_position = position + (speed * interpolation)

  if (player.movement.x_direction > 0) {
    float change = (float)PLAYER_MOVEMENT_SPEED * delta;
    player.location.x = player.location.x + change;
    printf("right - x: %f, change: %f delta: %f\n", player.location.x, change,
           delta);
  }
  if (player.movement.x_direction < 0) {
    player.location.x = player.location.x - (PLAYER_MOVEMENT_SPEED * delta);

    printf("left - x: %f, change: %f delta: %f\n", player.location.x,
           PLAYER_MOVEMENT_SPEED * delta, delta);
  }
  if (player.movement.y_direction > 0) {
    player.location.y = player.location.y + (PLAYER_MOVEMENT_SPEED * delta);
  }
  if (player.movement.y_direction < 0) {
    player.location.y = player.location.y - (PLAYER_MOVEMENT_SPEED * delta);
  }
  // Draw the sprite
  graphics_draw_sprite(player.location);
  graphics_render_frame();
}

void render(float delta) {
  graphics_draw_background();
  player_draw(delta);
  renders++;
}

static void player_step() {
  // Get input
  if (kbhit()) {
    int key = getch();
    if (key > 0) {
      switch (key) {
        case 'w':
          player.movement.y_direction = -1;
          break;
        case 's':
          player.movement.y_direction = 1;
          break;
        case 'a':
          player.movement.x_direction = -1;
          break;
        case 'd':
          player.movement.x_direction = 1;
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
}

void update() {
  player_step();
  updates++;
}

int main() {
  // Try to start graphics mode

  if (!graphics_start()) {
    graphics_stop();
    printf("\nERROR: Could not switch to VGA 640x480 16bpp mode.\n");
    return 1;
  }
  graphics_create_sprite();

  int key = 0;
  int32_t skip_ticks = UCLOCKS_PER_SEC / TARGET_FPS;
  uclock_t next_game_tick = uclock();
  uclock_t start_time = next_game_tick;
  int loops;
  float interpolation;
  while (!exit_now) {
    loops = 0;
    while (uclock() > next_game_tick && loops < MAX_FRAMESKIP) {
      process_input();
      update();
      next_game_tick += skip_ticks;
      loops++;
    }

    interpolation =
        (float)(uclock() + skip_ticks - next_game_tick) / (float)(skip_ticks);
    render(interpolation);
  }

  graphics_stop();
  uclock_t end_time = uclock();
  uclock_t total_time = end_time - start_time;
  float total_seconds = (float)total_time / (float)UCLOCKS_PER_SEC;
  float fps = (float)updates / total_seconds;
  printf("Total time was %f seconds (%d ticks)\n", total_seconds, total_time);
  printf("Rendered %d times and updated %d times - update FPS: %f \n", renders,
         updates, fps);

  return 0;
}