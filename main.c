#include <grx20.h>
//#include <grxkeys.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "int.h"

// Define Constants
#define COLOR_BACKGROUND 17
#define COLOR_PLAYER 9
#define PLAYER_MOVEMENT_DISTANCE 10
#define SQUARE_SIZE 10
#define TARGET_FPS 60
#define MAX_FRAMESKIP 5

int renders = 0;
int updates = 0;

void render(double delta) { renders++; }

void update() { updates++; }

int main() {
  static bool exit_now = false;
  int key = 0;
  int32_t skip_ticks = UCLOCKS_PER_SEC / TARGET_FPS;
  uclock_t next_game_tick = uclock();
  uclock_t start_time = next_game_tick;
  int loops;
  float interpolation;
  while (!exit_now) {
    if (kbhit()) {
      key = getch();
      if (key == 'a') {
        exit_now = true;
      }
    }

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
  uclock_t end_time = uclock();
  uclock_t total_time = end_time - start_time;
  float total_seconds = (float)total_time / (float)UCLOCKS_PER_SEC;
  float fps = (float)updates / total_seconds;
  printf("Total time was %f seconds (%d ticks)\n", total_seconds, total_time);
  printf("Rendered %d times and updated %d times - update FPS: %f \n", renders,
         updates, fps);
  return 0;
}