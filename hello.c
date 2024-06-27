/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.background = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";

  int x = 300;
  int y = 300;

  int vx = 1;
  int vy = 1;
  int r = 16;
  static vga_ball_color_t colors[] = {
    { 0x00, 0x00, 0x00, 0x9f, 0x00, 0x9f, 0x00, 0x01 }, /* Red */
    { 0x00, 0xff, 0x00, 0x9f, 0x00, 0x9f, 0x00, 0x01 }, /* Green */
    { 0x00, 0x00, 0xff, 0x9f, 0x00, 0x9f, 0x00, 0x01 }, /* Blue */
    { 0xff, 0xff, 0x00, 0x9f, 0x00, 0x9f, 0x00, 0x01 }, /* Yellow */
    { 0x00, 0xff, 0xff, 0x90, 0x00, 0x9f, 0x00, 0x01 }, /* Cyan */
    { 0xff, 0x00, 0xff, 0xA0, 0x00, 0x9f, 0x00, 0x01 }, /* Magenta */
    { 0x80, 0x80, 0x80, 0xB0, 0x00, 0x9f, 0x00, 0x01 }, /* Gray */
    { 0x00, 0x00, 0x00, 0xC0, 0x00, 0x9f, 0x00, 0x01 }, /* Black */
    { 0xff, 0xff, 0xff, 0xD0, 0x00, 0x9f, 0x00, 0x01 }  /* White */
  };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  printf("initial state: ");
  print_background_color();

  while(1){
    colors[i%COLORS].x_low = x;
    colors[i%COLORS].x_high = (x >> 5);
    colors[i%COLORS].y_low = y;
    colors[i%COLORS].y_high = (y >> 5);
    colors[i%COLORS].red = r;
    x+=vx;
    y+=vy;
    if(x-r <= 0 || x+r >= 639){
      vx = -vx;
      if(r>16) r--;
    }
    if(y+r >= 479 || y-r <= 0){
      vy = -vy;
      if(r>16) r--;
    }

    set_background_color(&colors[i % COLORS ]);
    usleep(10000);
  }
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
