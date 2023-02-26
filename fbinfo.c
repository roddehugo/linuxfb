#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  int fbfd;
  struct fb_var_screeninfo vinfo;

  /* Open the framebuffer device file for reading and writing. */
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    perror("Failed to open framebuffer device");
    exit(1);
  }
  printf("The framebuffer device opened.\n");

  /* Get variable screen information. */
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    close(fbfd);
    perror("Failed to read variable information");
    exit(2);
  }
  printf("%dx%d, %d bpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  printf("rotate=%d\n", vinfo.rotate);
  printf("activate=%d\n", vinfo.activate);

  /* Blank display. */
  if (ioctl(fbfd, FBIOBLANK, 0)) {
    close(fbfd);
    perror("Failed to blank display");
    exit(3);
  }

  /* Close file descriptor. */
  close(fbfd);

  return 0;
}
