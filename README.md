# absU Game Library

## What?
absU is a library for writing games in C that run on [DOSBox](https://dosbox.com). It aims to provide graphics, drawing primitives, sprites, tile maps, collision detection, input (keyboard/mouse/gamepad) handling, sound and a bunch of useful utilities. The library and programs that use it can currently only be compiled with [DJGPP](http://www.delorie.com/djgpp/), but [OpenWatcom](http://openwatcom.org/) will eventually be supported as well. It has no dependencies.

## Status
absU is in the very early stages of development, but a lot of things work. Here's a list. Anything crossed out is functional:
* Graphics
    * ~~VBE 2.0~~
    * ~~SVGA~~
    * ~~Screen buffering~~
    * ~~"blocks" for drawing and bitmap loading~~
    * ~~sprites~~
    * ~~tiles~~
    * animation
    * scrolling/parallax
* Drawing
    * ~~pixels~~
    * ~~horizontal/vertical lines~~
    * ~~filled/empty rectangles~~
    * circles and arcs
* ~~Bitmap loading (BMP files)~~
* Input handling
    * ~~keyboard~~
    * mouse
    * gamepad
* ~~Sound~~ (see below)
* Collision detection
* Simple 2D physics

## Installation
The following instructions assume you're doing your development on modern computer running a recent version of something UN*X-y, like Linux, OSX or Windows via something like WSL. You could probably figure out how to get it to work under Windows natively, but that's outside of the scope of this document. You could probably _also_ get it to work by having your entire development environment in DOSBox (I used to do this, actually) but again, you're on your own there. We'l be using DJGPP's cross compiler, which is a neat package that lets you target 32-bit DOS on a modern OS. You can get this running a number of different ways, including your distro's package manager, compiling it yourself, etc. My current favorite method (and by far the easiest) is a project called [build-djgpp](https://github.com/andrewwutw/build-djgpp). You can compile it yourself, but they also have prebuilt binaries for a number of platforms [here](https://github.com/andrewwutw/build-djgpp/releases).

**NOTE:** absU targets the _DOSBox_ emulator, not physical computers. Old PC hardware is becoming increasingly rare and fragile. If you want to fight collectors for "vintage hardware" on eBay so you can get it home and spend your weekend resolving IRQ conflicts, then hey, go for it. It would probably work fine on an actual 386 or 486 PC running DOS. It should also work fine in hardware emulators like [86Box](https://86box.net/) and [PCEm](https://pcem-emulator.co.uk/). I haven't tested any of that, don't plan to and have no plans to support anything that isn't DOSBox.

### Prerequisites
* [DOSBox](https://www.dosbox.com/)
* [DJGPP](http://www.delorie.com/djgpp/) installed as a cross-compiler. From here on we'll assume you got it from [here](https://github.com/andrewwutw/build-djgpp/releases) and that it's installed in `/usr/local/djgpp`. If you put it elsewhere that's fine, just make sure that `/usr/local/djgpp/bin` contains a bunch of files that start with something like `i586-pc-msdosdjgpp-`. (This may be different if you're using a 32-bit machine.)

### Building the test program
1. Clone this repo somewhere:

    `git clone git@github.com:jaybill/absu.git`

1. Change into the directory:

    `cd absu`

1. Run make to build the library and test programs:

    `make`

1. You can run the test program in `/bin/test` from DOSBox.

### Building as part of your project
You could just create a `deps/absu` folder in your project, drop the `src` and `include` directories from this repo into it and then compile everything as part of your application. You can easily adapt the `Makefile`, `run.bat` and `build.bat` to your build process.

You could also build it as a static library and link it to your program, but doing so is outside the scope of this document.

### Building as a shared library
DJGPP can't do that.

## Documentation
API documentation can be built with [Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html). The code is all commented and there's a `Doxyfile` in the root of the project. A bunch of tutorials are in the works.

### Sound Support

Sound is a complex beast. Simple PC speaker sound is pretty straightforward, but the SB16 I want to support in DOSBox is a whole different animal. I _could_ spend the time parsing out how that card works in enough detail to make my own driver, but research has shown this to be prohibitively complicated to the point where I've decided to skip it for the moment. Don't worry, though, I've provided everyone's favorite free, Satan-themed and somewhat orphaned sound library, [Judas](https://github.com/volkertb/JUDAS)! It's in the `vendor/JDAS209F` folder. What's included is the last non-GPL'd version, [2.09F](https://github.com/volkertb/JUDAS/blob/master/LICENSE.md), which is free to include in your project without having to supply your source code or your object files to the end user. I've shown how to compile and use it in the test program. If there's enough interest, I'll eventually add sound natively within the library. 

I should add that Judas is _by far_ the best solution I could find for sound in DOS games short of licensing the SDK from Creative, which I'm not even sure is possible anymore. I spent a literal week digging around for working example code or an existing library with some kind of permissive, BSD-style licensing and came up dry. This version of Judas is the closest I could get. It's a very nice library, and the version I've supplied is (as near as I can tell) legally unencumbered and can safely be used in commercial projects. While I'm sure you're in the clear, please note that I am not a lawyer and you should not trust anything I say about anything. I'm providing a copy of Judas for convenience, doing the right thing from a legal perspective is on you.

## What's with the name?
It's short for "Absolute Unit". It's in no way associated with the black metal band [Absu](http://absu.us). I didn't actually know about them until after I named the library, but I have since become a fan.

## Acknowledgements
See [THANKS.md](THANKS.md) in the root of this repo.

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

