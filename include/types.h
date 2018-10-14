///
/// Copyright (C) 2018, Jaybill McCarthy
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///

/* types.h */

#ifndef ABSU_TYPES_H
#define ABSU_TYPES_H

#define BYTE unsigned char

#define OK 0
#define ERR_CANT_GET_VESA_INFO 1
#define ERR_NO_LINEAR_FRAMEBUFFER 2
#define ERR_PHYSICAL_MAP_FAILURE 3
#define ERR_CANT_SET_MODE 4
#define ERR_CANT_OPEN_BITMAP_FILE 5
#define ERR_NOT_A_BITMAP 6
#define ERR_CANT_ALLOCATE_MEMORY 7

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

#endif  // !ABSU_TYPES_H