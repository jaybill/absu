# ABSU Game Library

## What?
ABSU is a library for writing games in C (or assembly language) that run on [DOSBox](https://dosbox.com) or Intel 80386 (or later) and 100% compatible based PCs running DOS (either [FreeDOS](http://www.freedos.org/) or [MS DOS](https://en.wikipedia.org/wiki/MS-DOS) ). It aims to provide graphics, sprites, input (keyboard/mouse/gamepad) handling, sound and a bunch of useful utilities. The library and programs that use it can be currently only be compiled with [DJGPP](http://www.delorie.com/djgpp/), but [OpenWatcom](http://openwatcom.org/) will eventually be supported as well. It has no dependencies.

## Installation
The following instructions assume you're doing your development on a modern (as of 2018) computer running a recent version of Windows, OSX or Linux and essentially treating DOS as an embedded system. While I'm certain you could get the tools and all working _within a DOS environment_, you're on your own setting that up. If you do end up doing that and want to help with this project, feel free to write up directions and I'll include them in this package.

### Prerequisites
* [build-djgpp](https://github.com/andrewwutw/build-djgpp) will get the DJGPP cross compiler set up on a modern computer. All the directions on how to get that running and the prerequisites for _that_ are over there. Any of the supported platforms should work just fine, but only the MinGW setup under Windows 10 has been tested. 
* One of the following, because your compiled programs will _not run_ on any recent version of Windows:
    * [DOSBox](https://www.dosbox.com/)
    * [FreeDOS] or [MS DOS] running under virtualization with [QEMU] or [VirtualBox]
    * An actual 386 PC running DOS, though doing this without doing one of the above is going to be _tedious_ as you'll have to get your programs onto said machine every time you compile them to run them.
* The environment variable `DJGPP_CC` must be set to the full path of the cross compiler. On a *NIX-y system (Linux, OSX, MSYS) you can do that with `export DJGPP_CC=/local/djgpp/i586-pc-msdosdjgpp/bin/gcc.exe` (Yours will probably be slightly different.)
* Some of the resource importing tools are written in [Python](https://www.python.org/), so if you want to use those, you'll need that.

### Building as part of your project
There's no reason you couldn't just create a `deps/absu` folder in your project, drop the `src` and `include` directories from this repo into it and then compile everything as part of the build process for your application. This is probably the easiest way to get going.

### Building as a static library
1. Clone this repo somewhere:

    git clone git@github.com:jaybill/absu.git

1. Change into the directory:

    cd absu

1. To build the library and the test program:

    make

1. To build just the library:

    make lib

1. Assuming that all worked, you can now use the library and the include files in your own project. 

### Building as a shared library
DJGPP can't do that.

## License

    Copyright (C) 2018, Jaybill McCarthy 

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the "Software"), 
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions: 

    The above copyright notice and this permission notice shall be included in 
    all copies or substantial portions of the Software. 

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE. 

