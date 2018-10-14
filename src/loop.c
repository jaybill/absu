/// Copyright (C) 2018, Jaybill McCarthy
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///

/* loop.c */

#include "../include/loop.h"

#include <stdint.h>
#include <time.h>

void loop_run(bool (*update)(), void (*render)(), int target_fps,
               int max_frameskip) {
  int32_t skip_ticks = UCLOCKS_PER_SEC / target_fps;
  uclock_t next_game_tick = uclock();
  uclock_t start_time = next_game_tick;
  uclock_t game_time = uclock();
  int loops, updates;
  bool exit_now = false;
  while (!exit_now) {
    game_time = uclock();
    loops = 0;
    while (game_time > next_game_tick && loops < max_frameskip) {
      exit_now = update();
      next_game_tick += skip_ticks;
      loops++;
      updates++;
    }
    render();
  }

}