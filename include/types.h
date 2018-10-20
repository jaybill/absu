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

/** @file types.h */

#ifndef ABSU_TYPES_H
#define ABSU_TYPES_H

#define BYTE unsigned char
#define WORD unsigned short

#define OK \
  0 /**< @brief Everything is fine. We're all fine here. How are you? */
#define ERR_CANT_GET_VESA_INFO \
  1 /**< @brief Error getting VESA info from the card */
#define ERR_NO_LINEAR_FRAMEBUFFER \
  2 /**< @brief The installed card does not have linear framebuffer mode */
#define ERR_PHYSICAL_MAP_FAILURE \
  3 /**< @brief Couldn't map to the physical video memory */
#define ERR_CANT_SET_MODE \
  4 /**< @brief The video card does not support the requested video mode */
#define ERR_CANT_OPEN_FILE 5 /**< @brief Can't open the file */
#define ERR_NOT_A_BITMAP \
  6 /**< @brief The file supplied is not a BMP bitmap file */
#define ERR_CANT_ALLOCATE_MEMORY \
  7 /**< @brief Can't allocate memory. Usually means there's not enough. */

#define ON 1  /**< @brief Use to represent an ON state */
#define OFF 0 /**< @brief Use to represent an OFF state */

#endif  // !ABSU_TYPES_H