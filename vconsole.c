#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/kd.h>

int main(int argc, char **argv)
{
    int fd;
  
    if (argc < 2)
    {
        fprintf(stderr, "Could not read virtual console device.\n");
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if (!fd)
    {
        fprintf(stderr, "Could not open virtual console.\n");
        return 1;
    }

    if (ioctl(fd, KDSETMODE, KD_GRAPHICS))
    {
        fprintf(stderr, "Could not set virtual console to KD_GRAPHICS mode.\n");
        return 1;
    }
}
