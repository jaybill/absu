/*
 * Copyright (C) 2018, Jaybill McCarthy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/** @file keyboard.h
 * @brief Functions for handling keyboard input directly via interrupt.
 * Adapted from examples written by Martynas Kunigelis in 1996.
 */

#ifndef ABSU_KEYBOARD_H
#define ABSU_KEYBOARD_H

/** @brief A continually updated array containing keys that are currently down.
 * Can be tested with the keyboard_key_down(scan) macro. Constants mapped to
 * scan codes are provided. */
extern volatile char keyboard_map[128];

/** @brief Installs keyboard handler. Returns OK on success or -1 on failure.*/
extern int keyboard_init(void);
/** @brief Removes keyboard handler. */
extern void keyboard_close(void);
/** @brief Makes the handler chain to previous keyboard routine if called ON.
 * Disables chaining if called with OFF. With chaining disabled, ^C, ^Break and
 * even ctrl-alt-del will not work. In addition, if you call keyboard_chain
 * to restore chaining, it clears BIOS shift/alt/control status bits at
 * 0400:0017. The reason for this is that if shift (or alt, or ctrl) key is
 * down when you call keyboard_chain(0), BIOS will think it still is after
 * you call keyboard_chain(1), because it never got to see you releasing
 * it. Thus, it is a good idea to restore chaining (if disabled) before
 * calling keyboard_close(), because you might get capitals or even alt or
 * control sequences when typing ordinary characters afterwards.
 */
extern void keyboard_chain(int toggle);
/** @brief returns 1 if supplied scan code is currently in the key map. Used to
 * test if a key is down. Can be used to test multiple keys at the same time.*/
#define keyboard_key_down(scan) ((int)keyboard_map[(scan)])

#define KEY_ESC 0x01
#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0a
#define KEY_0 0x0b
#define KEY_MINUS 0x0c
#define KEY_EQUALS 0x0d
#define KEY_BACKSP 0x0e
#define KEY_TAB 0x0f
#define KEY_Q 0x10
#define KEY_W 0x11
#define KEY_E 0x12
#define KEY_R 0x13
#define KEY_T 0x14
#define KEY_Y 0x15
#define KEY_U 0x16
#define KEY_I 0x17
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_LANGLE 0x1a
#define KEY_RANGLE 0x1b
#define KEY_ENTER 0x1c
#define KEY_CTRL 0x1d
#define KEY_A 0x1e
#define KEY_S 0x1f
#define KEY_D 0x20
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_SCOLON 0x27
#define KEY_QUOTA 0x28
#define KEY_RQUOTA 0x29
#define KEY_LSHIFT 0x2a
#define KEY_BSLASH 0x2b
#define KEY_Z 0x2c
#define KEY_X 0x2d
#define KEY_C 0x2e
#define KEY_V 0x2f
#define KEY_B 0x30
#define KEY_N 0x31
#define KEY_M 0x32
#define KEY_COMA 0x33
#define KEY_DOT 0x34
#define KEY_SLASH 0x35
#define KEY_RSHIFT 0x36
#define KEY_GREY_STAR 0x37
#define KEY_ALT 0x38
#define KEY_SPACE 0x39
#define KEY_CAPS 0x3a
#define KEY_F1 0x3b
#define KEY_F2 0x3c
#define KEY_F3 0x3d
#define KEY_F4 0x3e
#define KEY_F5 0x3f
#define KEY_F6 0x40
#define KEY_F7 0x41
#define KEY_F8 0x42
#define KEY_F9 0x43
#define KEY_F10 0x44
#define KEY_NUMLOCK 0x45
#define KEY_SCRLOCK 0x46
#define KEY_HOME 0x47
#define KEY_UP 0x48
#define KEY_PGUP 0x49
#define KEY_GREY_MINUS 0x4a
#define KEY_LEFT 0x4b
#define KEY_PAD_5 0x4c
#define KEY_RIGHT 0x4d
#define KEY_GREY_PLUS 0x4e
#define KEY_END 0x4f
#define KEY_DOWN 0x50
#define KEY_PGDN 0x51
#define KEY_INSERT 0x52
#define KEY_DEL 0x53
#define KEY_F11 0x57
#define KEY_F12 0x58

#endif  // !ABSU_KEYBOARD_H
