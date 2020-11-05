# Playground with the Linux Framebuffer

## What is a frame buffer device?

Taken from Geert Uytterhoeven's framebuffer.txt in the linux kernel sources:

> A framebuffer device is an abstraction for the graphic hardware. It represents
> the frame buffer of some video hardware, and allows application software to
> access the graphic hardware through a well-defined interface, so that the
> software doesn't need to know anything about the low-level interface stuff.

## Why use the frame buffer directly?

The frame buffer device is a very low-level interface to display something on
the screen. Speaking about an embedded GUI there are several reasons to use the
frame buffer directly instead of a Window manager:

- simple: just write the pixels to a memory
- fast: no window manager which means fast boot and less overhead
- portable: independently from the distribution every Linux system has a frame
buffer device so it's compatible with all of them

More detailed information can be found in the great manifest for Raspberry Pi
Wiki about the [framebuffer][framebuffer] and the [framebuffer
console][framebuffer-console].

## Compile examples

Single C source file should compile well on any Unix OS with `make` command:

    make [all|clean]

It leverages [Make implicit rules][make-implicit-rules].

### Credits

For `fbinfo`, see [low level graphics on Raspberry Pi][fbinfo]. For `fbmmap`,
see [enabling the linux framebuffer][fbmmap] from Qt docs.

## Commands

### Quickly output bytes to `linuxfb`

One may need `sudo` depending on the file permissions to run [dd][dd].
Error like `No space left on device` is expected.

    # Output random pixel noise
    dd if=/dev/urandom of=/dev/fb0

    # Clear framebuffer
    dd if=/dev/zero of=/dev/fb0

### Get information about a framebuffer

Print detailed information. Optional `fb` parameter to fall back on default.

    fbset -fb /dev/fb0 -i

Initialize the framebuffer if _disabled_ by a remapping of the framebuffer
console via a [kernel parameter][kernel-parameter]. Soft reset when coupled with
clear framebuffer command. A call like `con2fbmap 1 0` is needed to actually get
the framebuffer working with `linuxfb` API.

    fbset -fb /dev/fb0 "640x480-60"

See `/etc/fb.modes`.

### Disable cursor blinking in framebuffer console

    vt.global_cursor_default=0

### Disable framebuffer console before boot

Add this kernel parameter to disable console framebuffer `fb0`:

    fbcon=map:1

From [The Framebuffer Console][kernel-framebuffer-console]:

> 3 . fbcon=map:<0123>

> This is an interesting option. It tells which driver gets mapped to which
> console. The value '0123' is a sequence that gets repeated until the total
> length is 64 which is the number of consoles available. In the above example,
> it is expanded to 012301230123... and the mapping will be:

>     tty | 1 2 3 4 5 6 7 8 9 ...
>     fb  | 0 1 2 3 0 1 2 3 0 ...

> (`cat /proc/fb` should tell you what the fb numbers are)

> One side effect that may be useful is using a map value that exceeds the
> number of loaded fb drivers. For example, if only one driver is available,
    > `fb0`, adding `fbcon=map:1` tells fbcon not to take over the console.

> Later on, when you want to map the console the to the framebuffer device, you
> can use the `con2fbmap` utility.

More details about the [con2fbmap utility][con2fbmap].
With hints here on [where to put such a piece of code][etc-rc-local].

## TFT LCD display modules

As per [Linux Framebuffer drivers for small TFT LCD display][git-notro-fbtft]:

> 2015-01-19
> The FBTFT drivers are now in the Linux kernel staging tree:
> https://git.kernel.org/cgit/linux/kernel/git/gregkh/staging.git/tree/drivers/staging/fbtft?h=staging-testing
> Development in this github repo has ceased.

If one wants to read someone struggling to setup its [LCD driver at
boot][lcd-driver-boot], as of Octobre 2015. Hence a little outdated.

## Going further

Read the [Framebuffer API][kernel-framebuffer-api] official documentation:

> This document describes the frame buffer API used by applications to interact with
> frame buffer devices. In-kernel APIs between device drivers and the frame buffer
> core are not described.

> Due to a lack of documentation in the original frame buffer API, drivers
> behaviours differ in subtle (and not so subtle) ways. This document describes the
> recommended API implementation, but applications should be prepared to deal with
> different behaviours

Read the [Framebuffer HOWTO][framebuffer-howto] by Alex Buell (2010):

> This document describes how to use the framebuffer devices in Linux with a
> variety of platforms. This also includes how to set up multi-headed
> displays.

Some chapters that are interesting in our case:

    Table of Contents
    1. Contributors
    2. What is a framebuffer device?
    3. What advantages does framebuffer devices have?
    4. Using framebuffer devices on x86 platforms
    10. Using framebuffer devices on ARM platforms
    13. Changing Console Modes
    17. Looking for further information

[make-implicit-rules]:https://www.gnu.org/software/make/manual/html_node/Implicit-Rules.html
[fbinfo]:http://raspberrycompote.blogspot.com/2012/12/low-level-graphics-on-raspberry-pi-part_9509.html
[fbmmap]:https://doc.qt.io/archives/3.3/emb-framebuffer-howto.html
[lfb-commandline]:https://unix.stackexchange.com/questions/192206
[framebuffer-console]:https://github.com/FrankBau/raspi-repo-manifest/wiki/fbcon
[framebuffer]:https://github.com/FrankBau/raspi-repo-manifest/wiki/framebuffer
[kernel-parameter]:https://github.com/FrankBau/raspi-repo-manifest/wiki/kernel-parameter
[dd]:https://github.com/FrankBau/raspi-repo-manifest/wiki/dd
[kernel-framebuffer-api]:https://www.kernel.org/doc/Documentation/fb/api.txt
[kernel-framebuffer-console]:https://www.kernel.org/doc/Documentation/fb/fbcon.txt
[con2fbmap]:https://github.com/notro/fbtft/wiki/Framebuffer-use#console
[etc-rc-local]:https://www.raspberrypi.org/forums/viewtopic.php?t=222402
[git-notro-fbtft]:https://github.com/notro/fbtft
[lcd-driver-boot]:https://www.raspberrypi.org/forums/viewtopic.php?t=123676
[framebuffer-howto]:https://www.tldp.org/HOWTO/text/Framebuffer-HOWTO
