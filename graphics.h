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
GrColor *graphics_allocate_palette(void);

#endif  // !MW_GRAPHICS_H