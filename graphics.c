#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>

#define PI 3.141592653589793
#define BITS_PER_PIXEL 32
#define CIRCLE_SEGMENTS 8

int fb;
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
int screenwidth;
int screenheight;
long screensize;
uint8_t* fbptr;
int argb;

void gfx_start(char* file) {
  fb = open(file, O_RDWR);
  ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
  ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
  screenwidth = vinfo.xres;
  screenheight = vinfo.yres;
  vinfo.bits_per_pixel = BITS_PER_PIXEL;
  ioctl(fb, FBIOPUT_VSCREENINFO, &vinfo);
  screensize = vinfo.yres_virtual * finfo.line_length;
  fbptr = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, (off_t) 0);
}

void gfx_setColor(int _argb) {
  argb = _argb;
}

void gfx_setPixel(int x, int y) {
  if (x < 0 || x >= vinfo.xres || y < 0 || y >= vinfo.yres) {
    return;
  }
  long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
  *((uint32_t*) (fbptr + location)) = argb;
}

void gfx_drawLine(int x1, int y1, int x2, int y2) {
  int dx = x2 - x1;
  int dy = y2 - y1;
  /*if (dx != 0 && dy / dx > 1) {
    int x;
    for (x = 0; x <= dx; ++x) {
      gfx_setPixel(x1 + x, (int) (y1 + (dy / (double) dx * x)));
    }
  } else if (dx >= 0 && dy / dx <= 1) {
    int y;
    for (y = 0; y <= dy; ++y) {
      gfx_setPixel(x1 + (int) (x1 + (dx / (double) dy * y)), y1 + y);
    }
  } else if (dy != 0 && dx < 0 && dy / dx <= -1) {
    int y;
    for (y = 0; y >= dy; --y) {
      gfx_setPixel(x1 + (int) (x1 + (dx / (double) dy * y)), y1 + y);
    }
  } else if (dy / dx > -1) {
    int x;
    for (x = 0; x >= dx; --x) {
      gfx_setPixel(x1 + x, (int) (y1 + (dy / (double) dx * x)));
    }
  }*/
  int x;
  for (x = 0; x <= dx; x++) {
    gfx_setPixel(x1 + x, y1);
  }
}

void gfx_drawRect(int x1, int y1, int x2, int y2) {
  gfx_drawLine(x1, y1, x1, y2);
  gfx_drawLine(x2, y1, x2, y2);
  gfx_drawLine(x1, y1, x2, y1);
  gfx_drawLine(x1, y2, x2, y2);
}

void gfx_fillRect(int x1, int y1, int x2, int y2) {
  int dx = x2 - x1;
  int x;
  for (x = 0; x <= dx; ++x) {
    gfx_drawLine(x1 + x, y1, x1 + x, y2);
  }
}

void gfx_drawOval(int x1, int y1, int x2, int y2) {
  int dx = x2 - x1;
  int dy = y2 - y1;
  int v, w;
  int x, y;
  double angle;
  double delta = 2 * PI / CIRCLE_SEGMENTS;
  for (angle = 0; angle < 2 * PI; angle += delta) {
    v = (cos(angle + delta) / 2 + 1) * dx;
    w = (sin(angle + delta) / 2 + 1) * dy;
    x = (cos(angle) / 2 + 1) * dx;
    y = (sin(angle) / 2 + 1) * dy;
    //gfx_setPixel(x1 + x, y1 + y);
    gfx_drawLine(x1 + v, y1 + w, x1 + x, y1 + y);
  }
}

void gfx_fillOval(int x1, int y1, int x2, int y2) {
  int dx = x2 - x1;
  int dy = y2 - y1;
  int y;
  double angle;
  for (y = 0; y <= dy; ++y) {
    angle = asin((y / (double) dy) * 2 - 1);
    gfx_drawLine(x1 + (-cos(angle) / 2 + 1) * dx, y1 + y, x1 + (cos(angle) / 2 + 1) * dx, y1 + y);
  }
}
