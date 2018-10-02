#ifndef MW_GRAPHICS_H
#define MW_GRAPHICS_H
#include <mgrx.h>
#include <stdbool.h>
#include <stdint.h>

extern GrColor *egacolors;
#define BLACK egacolors[0]
#define BLUE egacolors[1]
#define GREEN egacolors[2]
#define CYAN egacolors[3]
#define RED egacolors[4]
#define MAGENTA egacolors[5]
#define BROWN egacolors[6]
#define LIGHTGRAY egacolors[7]
#define DARKGRAY egacolors[8]
#define LIGHTBLUE egacolors[9]
#define LIGHTGREEN egacolors[10]
#define LIGHTCYAN egacolors[11]
#define LIGHTRED egacolors[12]
#define LIGHTMAGENTA egacolors[13]
#define YELLOW egacolors[14]
#define WHITE egacolors[15]

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

struct graphics_point {
  int32_t x, y;
};

struct graphics_rect {
  struct graphics_point tl, br;
};

bool graphics_start();
void graphics_stop();
void graphics_create_sprite();
void graphics_draw_rectangle(struct graphics_rect rect);
void graphics_render_frame();

#endif  // !MW_GRAPHICS_H