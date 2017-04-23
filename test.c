#include "graphics.h"

int main() {
  gfx_start("/dev/fb0");
  gfx_setColor(0x000000FF); // ARGB for tv, RGBA for monitor
  gfx_fillRect(0, 0, 1920, 1080);
  gfx_setColor(0x0000FF00);
  gfx_drawLine(0, 0, 100, 100);
  gfx_drawLine(150, 100, 500, 200);
  gfx_drawLine(0, 0, 100, 500);
  gfx_drawRect(50, 50, 200, 500);
  gfx_drawOval(500, 500, 800, 800);
  return 0;
}
