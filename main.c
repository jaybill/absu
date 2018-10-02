#include <conio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "graphics.h"

// Define Constants
#define COLOR_BACKGROUND BLACK
#define COLOR_PLAYER MAGENTA
#define PLAYER_MOVEMENT_DISTANCE 10
#define SQUARE_SIZE 10
#define TARGET_FPS 60
#define MAX_FRAMESKIP 5

int renders = 0;
int updates = 0;
static bool exit_now = false;

struct player {
  int32_t x, y, dx, dy;
  struct graphics_rect rect;
  bool clear;
};

struct player player;

static void player_draw(double delta) {
  // Set up new square
  struct graphics_point rt2 = {player.x, player.y};
  struct graphics_point bl2 = {player.x + SQUARE_SIZE, player.y + SQUARE_SIZE};
  struct graphics_rect draw_rect = {rt2, bl2};

  // Save this rectangle data so we can clear it later
  player.rect = draw_rect;

  // Draw new square
  graphics_draw_rectangle(draw_rect);
  graphics_render_frame();
}

void render(double delta) {
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
          player.dy = -PLAYER_MOVEMENT_DISTANCE;
          break;
        case 's':
          player.dy = PLAYER_MOVEMENT_DISTANCE;
          break;
        case 'a':
          player.dx = -PLAYER_MOVEMENT_DISTANCE;
          break;
        case 'd':
          player.dx = PLAYER_MOVEMENT_DISTANCE;
          break;
        case 27:
          exit_now = true;
          break;
      }
    }
  }
  // Update x and y position of player, clamping movement to within screen
  if (player.x + player.dx > -1 &&
      player.x + player.dx < SCREEN_WIDTH - SQUARE_SIZE) {
    player.x = player.x + player.dx;
  } else if ((player.dx + player.x) - SQUARE_SIZE > SCREEN_WIDTH) {
    player.x = SCREEN_WIDTH - SQUARE_SIZE;
  }

  if (player.y + player.dy > -1 &&
      player.y + player.dy < SCREEN_HEIGHT - SQUARE_SIZE) {
    player.y = player.y + player.dy;
  } else if ((player.dy + player.y) - SQUARE_SIZE > SCREEN_HEIGHT) {
    player.y = SCREEN_HEIGHT - SQUARE_SIZE;
  }

  player.dy = 0;
  player.dx = 0;
}

void update() {
  player_step();
  updates++;
}

int main() {
  // Try to start graphics mode

  if (!graphics_start()) {
    graphics_stop();

    printf("\nERROR: Can't switch to VGA 320x200 8bpp\n");
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