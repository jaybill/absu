#include "graphics.h"
#include <stddef.h>

GrContext *graphics_grc;
GrColor *egacolors;

bool graphics_start() {
  if (!GrSetMode(GR_320_200_graphics)) {
    return false;
  }
  egacolors = GrAllocEgaColors();

  GrClearScreen(GrBlack());
  return true;
}

void graphics_stop() { GrSetMode(GR_320_200_graphics); }

void graphics_draw_rectangle(struct graphics_rect rect, GrColor color) {
  GrFilledBox(rect.tl.x, rect.tl.y, rect.br.x, rect.br.y, color);
}