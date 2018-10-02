#include "graphics.h"
#include <stddef.h>
#include <stdio.h>

GrContext *sprite;
GrContext *screen_buffer;
GrColor *egacolors;

bool graphics_start() {
  if (!GrSetMode(GR_320_200_graphics)) {
    return false;
  }
  egacolors = GrAllocEgaColors();
  GrClearScreen(GrBlack());
  screen_buffer = GrCreateContext(SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL);
  GrSetContext(screen_buffer);
  GrClearContext(BLACK);
  return true;
}

void graphics_stop() {
  GrSetContext(GrScreenContext());
  GrDestroyContext(screen_buffer);
  GrDestroyContext(sprite);
  GrSetMode(GR_TEXT_DEFAULT);
}

void graphics_create_sprite() {
  sprite = GrCreateContext(10, 10, NULL, NULL);
  GrSetContext(sprite);
  GrFilledBox(0, 0, 9, 9, RED);
  GrFilledBox(0, 0, 9, 1, WHITE);
  GrFilledBox(0, 8, 9, 9, WHITE);
  GrSaveContext(sprite);
  GrSetContext(GrScreenContext());
}

void graphics_draw_rectangle(struct graphics_rect rect) {
  GrBitBlt(screen_buffer, rect.tl.x, rect.tl.y, sprite, 0, 0, 10, 10, GrWRITE);
}

void graphics_render_frame() {
  GrBitBlt(GrScreenContext(), 0, 0, screen_buffer, 0, 0, SCREEN_WIDTH,
           SCREEN_HEIGHT, GrWRITE);
}
