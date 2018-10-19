# absU Game Library

## What?
absU is a library for writing games in C (or assembly language) that run on [DOSBox](https://dosbox.com) or Intel 80386 (or later) and 100% compatible based PCs running DOS (either [FreeDOS](http://www.freedos.org/) or [MS DOS](https://en.wikipedia.org/wiki/MS-DOS) ). It aims to provide graphics, drawing primitives, sprites, tile maps, collision detection, input (keyboard/mouse/gamepad) handling, sound and a bunch of useful utilities. The library and programs that use it can currently only be compiled with [DJGPP](http://www.delorie.com/djgpp/), but [OpenWatcom](http://openwatcom.org/) will eventually be supported as well. It has no dependencies.

## Installation
The following instructions assume you're doing your development on a modern (as of 2018) computer running a recent version of Windows, OSX or Linux but that you'll actually compile your code [DOSBox](https://www.dosbox.com/). Currently these instructions pertain specifically to Windows 10, but they should be broadly applicable to whatever OS you're using with pretty minor tweaking. While I'm certain you could get the tools and all working using DJGPP's cross compiler, you're on your own setting that up. There's a project called [build-djgpp](https://github.com/andrewwutw/build-djgpp) that can help with this, but as of this writing it only supports up to version 7.2.0 of DJGPP and it hasn't been updated for a year. You could, of course, do all of this on an actual 386 or 486 PC running DOS if that's your thing. Most of this will still apply.

### Prerequisites
* [DOSBox](https://www.dosbox.com/)
* [DJGPP](http://www.delorie.com/djgpp/) installed and configured in your DOSBox instance. You'll need at least the basic binutils, C and C++ packages.

### Building the test program
1. Clone this repo somewhere:

    `git clone git@github.com:jaybill/absu.git`

1. Change into the directory:

    `cd absu`

1. Create a directory for your DOS drive. Let's assume you create `C:\dosdrives\c_drive`

1. Create a directory inside that one for DJGPP. Let's say it's `C:\dosdrives\c_drive\djgpp`

1. Unpack the DJGPP archives 

1. Assuming DOSBox is installed, edit your config file. This will generally be in ` $HOME\AppData\Local\DOSBox` and be named something like `dosbox-xx.conf`. Add the following lines to the end of the file in the `[autoexec]` section. 
    ```
    MOUNT C C:\dosdrives\drive_c
    MOUNT F . -t floppy
    SET PATH=C:\DJGPP\BIN;%PATH
    SET DJGPP=C:\DJGPP\DJGPP.ENV
    F:
    ```
1. `build.bat` and `run.bat` are batch files intended to run on modern Windows. They launch DOSBox and supply it with the commands to build/run the test program. If you're using Linux or OSX you'll probably need to create a shell script that does the same thing. It should be almost identical to the batch file, as the commands are all run _inside_ DOSBox. Both batch files assume you have the DOSBox executable in your path.

1. Run `build.bat` from your host operating system. You should see DOSBox open, build the test application and then run it. To run the test application with [DOS94](http://dos94.com) settings, use `run.bat`

### Building as part of your project
You couldn't just create a `deps/absu` folder in your project, drop the `src` and `include` directories from this repo into it and then compile everything as part of your application. You can easily adapt the `Makefile`, `run.bat` and `build.bat` to your build process.

You could also build it as a static library and link it to your program, but doing so is outside the scope of this document.

### Building as a shared library
DJGPP can't do that.

## What's with the name?
It's short for "Absolute Unit". It's in no way associated with the black metal band [Absu](http://absu.us). I didn't actually know about them until after I named the library, but I have since become a fan.

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

