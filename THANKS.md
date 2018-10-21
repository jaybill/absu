# Special Thanks
Probably the single hardest thing about writing code that runs in DOS in 2018 was finding documentation and sample code for things that haven't been in regular use for 25 years. The web was just a young thing in 1994, and while there was some information on it that's still around, there's a _lot_ of link rot. There were plenty of times where I'd find some archived page that had what looked like the exact information I needed only to find the link to the zip file with the source code was long dead. 

A lot of the research I did involved digging in newsgroup archives (thanks, [Google Groups](https://groups.google.com)!) and rooting through incomplete mirrors of the now defunct [Simtel Archive](https://en.wikipedia.org/wiki/Simtel). I also read a number of honest to dog, dead tree _books_, which I've listed below.

I thought it would be right and good to mention some of the example code I found while working on this and thank those developers who kindly put said code in the public domain for me to find all these years later. In most cases, if they supplied an email, it's with some long forgotten ISP from the mid 90s. Often all I had was a name. I will put those names here and publicly thank those people. I have valid emails for some of these people, and some of them I've even communicated with. I'm not going to give you their emails here as I don't want anyone bothering them. (Bother me instead.) For those people I haven't been able to find, if you know them (or are them), please pass along my thanks and maybe send them a link to the repo. I imagine they'd be chuffed to know their ancient code is still helping people.

## People

* **Alexander Russell** wrote a [great series of tutorials and a library](http://www3.telus.net/alexander_russell/). I used a lot of his ideas here.
* **David Brackeen** has an [excellent site about DOS game development](http://www.brackeen.com/vga/) I referred to often.
* **Charles Sandmann** wrote an example of how to use VBE 2.0 linear memory that was absolutely vital to my understanding of the subject. His code serves as the basis of the _video.c_ and _video.h_.
* **Martynas Kunigelis** wrote an example of how to do hardware interrupt handing for keyboard input in C and assembly that's the basis of _keyboard.s_ and _keyboard.h_. I could not get my head around how to do this correctly before I saw his code.
* **DJ Delorie** is my hero. He ported the GCC and a good deal of the GNU ecosystem to DOS because [Richard Stallman told him it wasn't possible](http://www.delorie.com/djgpp/doc/eli-m17n99.html#History). [DJGPP](http://www.delorie.com/djgpp/) is an absolutely wonderful toolchain to work with and a large ecosystem of compatible libraries and example code is still floating around. DJ and friends still maintain DJGPP to this day.

## Books
* [Programmer's Guide to the EGA, VGA, and Super VGA Cards (3rd Edition)](https://smile.amazon.com/gp/product/0201624907) - Richard F. Ferraro
* [PC Interrupts : A Programmer's Reference to BIOS, DOS, and Third-Party Calls](https://smile.amazon.com/gp/product/0201624850) - Ralf Brown
* [C Programming Language, 2nd Edition](https://smile.amazon.com/gp/product/0131103628) - Brian W. Kernighan, Dennis M. Ritchie (I feel a little silly putting this here, as it's like a carpenter thanking hammers for help building a house. That said, this is _the_ C book and if you don't have it, go find a copy right now.)