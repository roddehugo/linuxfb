#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  int fbfd;
  char *fbp;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;

  /* Open the file for reading and writing. */
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    perror("Failed to open framebuffer device");
    exit(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  /* Get fixed screen information. */
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
    close(fbfd);
    perror("Failed to read fixed information");
    exit(2);
  }

  /* Get variable screen information. */
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
    close(fbfd);
    perror("Failed to read variable information");
    exit(3);
  }

  printf("%dx%d, %d bpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  printf("rotate=%d\n", vinfo.rotate);
  printf("activate=%d\n", vinfo.activate);

  /* Figure out the size of the screen in bytes. */
  long int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

  /* Map the device to memory. */
  fbp =
      (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((long)fbp == -1) {
    close(fbfd);
    perror("Failed to map framebuffer device to memory");
    exit(4);
  }
  printf("The framebuffer device was mapped to memory successfully.\n");

  /* Paint a pretty screen. */
  unsigned int x, y;
  long int location;
  for (y = 0; y < vinfo.yres; y++) {
    for (x = 0; x < vinfo.xres; x++) {
      location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                 (y + vinfo.yoffset) * finfo.line_length;

      if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = 100;                     // Some blue.
        *(fbp + location + 1) = 15 + (x - 100) / 2;  // A little green.
        *(fbp + location + 2) = 200 - (y - 100) / 5; // A lot of red.
        *(fbp + location + 3) = 0;                   // No transparency.
      } else                                         // Assume 16 bpp.
      {
        int b = 10;                  // Some blue.
        int g = (x - 100) / 6;       // A little green.
        int r = 31 - (y - 100) / 16; // A lot of red.
        unsigned short int t = r << 11 | g << 5 | b;
        *((unsigned short int *)(fbp + location)) = t;
      }
    }
  }
  printf("The framebuffer device was painted successfully.\n");

  /* Close memory mapped and file descriptor. */
  munmap(fbp, screensize);
  close(fbfd);

  return 0;
}
