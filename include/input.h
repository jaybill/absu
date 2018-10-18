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

/** @file input.h
 * @brief functions for getting keyboard input
 */

#ifndef ABSU_INPUT_H
#define ABSU_INPUT_H
#define KEY_A 0x1E61
#define KEY_B 0x3062
#define KEY_C 0x2E63
#define KEY_D 0x2064
#define KEY_E 0x1265
#define KEY_F 0x2166
#define KEY_G 0x2267
#define KEY_H 0x2368
#define KEY_I 0x1769
#define KEY_J 0x246A
#define KEY_K 0x256B
#define KEY_L 0x266C
#define KEY_M 0x326D
#define KEY_N 0x316E
#define KEY_O 0x186F
#define KEY_P 0x1970
#define KEY_Q 0x1071
#define KEY_R 0x1372
#define KEY_S 0x1F73
#define KEY_T 0x1474
#define KEY_U 0x1675
#define KEY_V 0x2F76
#define KEY_W 0x1177
#define KEY_X 0x2D78
#define KEY_Y 0x1579
#define KEY_Z 0x2C7A
#define KEY_1 0x0231
#define KEY_2 0x0332
#define KEY_3 0x0433
#define KEY_4 0x0534
#define KEY_5 0x0635
#define KEY_6 0x0736
#define KEY_7 0x0837
#define KEY_8 0x0938
#define KEY_9 0x0A39
#define KEY_0 0x0B30
#define KEY_MINUS 0x0C2D
#define KEY_EQUAL 0x0D3D
#define KEY_SQ_BRACKET_L 0x1A5B
#define KEY_SQ_BRACKET_R 0x1B5D
#define KEY_SEMICOLON 0x273B
#define KEY_BACKTICK 0x2960
#define KEY_BACKSLASH 0x2B5C
#define KEY_COMMA 0x332C
#define KEY_PERIOD 0x342E
#define KEY_FORWARDSLASH 0x352F
#define KEY_F1 0x3B00
#define KEY_F2 0x3C00
#define KEY_F3 0x3D00
#define KEY_F4 0x3E00
#define KEY_F5 0x3F00
#define KEY_F6 0x4000
#define KEY_F7 0x4100
#define KEY_F8 0x4200
#define KEY_F9 0x4300
#define KEY_F10 0x4400
#define KEY_F11 0x8500
#define KEY_F12 0x8600
#define KEY_BACKSPACE 0x0E08
#define KEY_DELETE 0x5300
#define KEY_DOWN_ARROW 0x5000
#define KEY_END 0x4F00
#define KEY_ENTER 0x1C0D
#define KEY_ESC 0x011B
#define KEY_HOME 0x4700
#define KEY_INS 0x5200
#define KEY_KEYPAD_5 0x4C35
#define KEY_KEYPAD_MULTIPLY 0x372A
#define KEY_KEYPAD_MINUS 0x4A2D
#define KEY_KEYPAD_PLUS 0x4E2B
#define KEY_KEYPAD_DIVIDE 0x352F
#define KEY_LEFT_ARROW 0x4B00
#define KEY_PG_DN 0x5100
#define KEY_PG_UP 0x4900
#define KEY_PRT_SC 0x7200
#define KEY_RIGHT_ARROW 0x4D00
#define KEY_SPACE_BAR 0x3920
#define KEY_TAB 0x0F09
#define KEY_UP_ARROW 0x4800
#define KEY_SHIFT_A 0x1E41
#define KEY_SHIFT_B 0x3042
#define KEY_SHIFT_C 0x2E42
#define KEY_SHIFT_D 0x2044
#define KEY_SHIFT_E 0x1245
#define KEY_SHIFT_F 0x2146
#define KEY_SHIFT_G 0x2247
#define KEY_SHIFT_H 0x2348
#define KEY_SHIFT_I 0x1749
#define KEY_SHIFT_J 0x244A
#define KEY_SHIFT_K 0x254B
#define KEY_SHIFT_L 0x264C
#define KEY_SHIFT_M 0x324D
#define KEY_SHIFT_N 0x314E
#define KEY_SHIFT_O 0x184F
#define KEY_SHIFT_P 0x1950
#define KEY_SHIFT_Q 0x1051
#define KEY_SHIFT_R 0x1352
#define KEY_SHIFT_S 0x1F53
#define KEY_SHIFT_T 0x1454
#define KEY_SHIFT_U 0x1655
#define KEY_SHIFT_V 0x2F56
#define KEY_SHIFT_W 0x1157
#define KEY_SHIFT_X 0x2D58
#define KEY_SHIFT_Y 0x1559
#define KEY_SHIFT_Z 0x2C5A
#define KEY_SHIFT_1 0x0221
#define KEY_SHIFT_2 0x0340
#define KEY_SHIFT_3 0x0423
#define KEY_SHIFT_4 0x0524
#define KEY_SHIFT_5 0x0625
#define KEY_SHIFT_6 0x075E
#define KEY_SHIFT_7 0x0826
#define KEY_SHIFT_8 0x092A
#define KEY_SHIFT_9 0x0A28
#define KEY_SHIFT_0 0x0B29
#define KEY_SHIFT_MINUS 0x0C5F
#define KEY_SHIFT_EQUAL 0x0D2B
#define KEY_SHIFT_SQ_BRACKET_L 0x1A7B
#define KEY_SHIFT_SQ_BRACKET_R 0x1B7D
#define KEY_COLON 0x273A
#define KEY_TILDA 0x297A
#define KEY_SHIFT_BACKSLASH 0x2B7C
#define KEY_LESS_THAN 0x333C
#define KEY_GREATER_THAN 0x343E
#define KEY_QUESTION_MARK 0x353F
#define KEY_SHIFT_F1 0x5400
#define KEY_SHIFT_F2 0x5500
#define KEY_SHIFT_F3 0x5600
#define KEY_SHIFT_F4 0x5700
#define KEY_SHIFT_F5 0x5800
#define KEY_SHIFT_F6 0x5900
#define KEY_SHIFT_F7 0x5A00
#define KEY_SHIFT_F8 0x5B00
#define KEY_SHIFT_F9 0x5C00
#define KEY_SHIFT_F10 0x5D00
#define KEY_SHIFT_F11 0x8700
#define KEY_SHIFT_F12 0x8800
#define KEY_SHIFT_BACKSPACE 0x0E08
#define KEY_SHIFT_DELETE 0x532E
#define KEY_SHIFT_DOWN_ARROW 0x5032
#define KEY_SHIFT_END 0x4F31
#define KEY_SHIFT_ENTER 0x1C0D
#define KEY_SHIFT_ESC 0x011B
#define KEY_SHIFT_HOME 0x4737
#define KEY_SHIFT_INS 0x5230
#define KEY_SHIFT_KEYPAD_5 0x4C35
#define KEY_SHIFT_KEYPAD_MULTIPLY 0x37
#define KEY_SHIFT_KEYPAD_MINUS 0x4A2D
#define KEY_SHIFT_KEYPAD_PLUS 0x4E2B
#define KEY_SHIFT_KEYPAD_DIVIDE 0x352F
#define KEY_SHIFT_LEFT_ARROW 0x4B34
#define KEY_SHIFT_PG_DN 0x5133
#define KEY_SHIFT_PG_UP 0x4939
#define KEY_SHIFT_RIGHT_ARROW 0x4D36
#define KEY_SHIFT_SPACE_BAR 0x3920
#define KEY_SHIFT_TAB 0x0F00
#define KEY_SHIFT_UP_ARROW 0x4838
#define KEY_CTRL_A 0x1E01
#define KEY_CTRL_B 0x3002
#define KEY_CTRL_C 0x2E03
#define KEY_CTRL_D 0x2004
#define KEY_CTRL_E 0x1205
#define KEY_CTRL_F 0x2106
#define KEY_CTRL_G 0x2207
#define KEY_CTRL_H 0x2308
#define KEY_CTRL_I 0x1709
#define KEY_CTRL_J 0x240A
#define KEY_CTRL_K 0x250B
#define KEY_CTRL_L 0x260C
#define KEY_CTRL_M 0x320D
#define KEY_CTRL_N 0x310E
#define KEY_CTRL_O 0x180F
#define KEY_CTRL_P 0x1910
#define KEY_CTRL_Q 0x1011
#define KEY_CTRL_R 0x1312
#define KEY_CTRL_S 0x1F13
#define KEY_CTRL_T 0x1414
#define KEY_CTRL_U 0x1615
#define KEY_CTRL_V 0x2F16
#define KEY_CTRL_W 0x1117
#define KEY_CTRL_X 0x2D18
#define KEY_CTRL_Y 0x1519
#define KEY_CTRL_Z 0x2C1A
#define KEY_CTRL_2 0x0300
#define KEY_CTRL_6 0x071E
#define KEY_CTRL_MINUS 0x0C1F
#define KEY_CTRL_SQ_BRACKET_L 0x1A1B
#define KEY_CTRL_SQ_BRACKET_R 0x1B1D
#define KEY_CTRL_BACKSLASH 0x2B1C
#define KEY_CTRL_F1 0x5E00
#define KEY_CTRL_F2 0x5F00
#define KEY_CTRL_F3 0x6000
#define KEY_CTRL_F4 0x6100
#define KEY_CTRL_F5 0x6200
#define KEY_CTRL_F6 0x6300
#define KEY_CTRL_F7 0x6400
#define KEY_CTRL_F8 0x6500
#define KEY_CTRL_F9 0x6600
#define KEY_CTRL_F10 0x6700
#define KEY_CTRL_F11 0x8900
#define KEY_CTRL_F12 0x8A00
#define KEY_CTRL_BACKSPACE 0x0E7F
#define KEY_CTRL_DELETE 0x9300
#define KEY_CTRL_DOWN_ARROW 0x9100
#define KEY_CTRL_END 0x7500
#define KEY_CTRL_ENTER 0x1C0A
#define KEY_CTRL_ESC 0x011B
#define KEY_CTRL_HOME 0x7700
#define KEY_CTRL_INS 0x9200
#define KEY_CTRL_KEYPAD_5 0x8F00
#define KEY_CTRL_KEYPAD_MULTIPLY 0x9600
#define KEY_CTRL_KEYPAD_MINUS 0x8E00
#define KEY_CTRL_KEYPAD_DIVIDE 0x9500
#define KEY_CTRL_LEFT_ARROW 0x7300
#define KEY_CTRL_PG_DN 0x7600
#define KEY_CTRL_PG_UP 0x8400
#define KEY_CTRL_PRT_SC 0x7200
#define KEY_CTRL_RIGHT_ARROW 0x7400
#define KEY_CTRL_SPACE_BAR 0x3920
#define KEY_CTRL_TAB 0x9400
#define KEY_CTRL_UP_ARROW 0x8D00
#define KEY_ALT_A 0x1E00
#define KEY_ALT_B 0x3000
#define KEY_ALT_C 0x2E00
#define KEY_ALT_D 0x2000
#define KEY_ALT_E 0x1200
#define KEY_ALT_F 0x2100
#define KEY_ALT_G 0x2200
#define KEY_ALT_H 0x2300
#define KEY_ALT_I 0x1700
#define KEY_ALT_J 0x2400
#define KEY_ALT_K 0x2500
#define KEY_ALT_L 0x2600
#define KEY_ALT_M 0x3200
#define KEY_ALT_N 0x3100
#define KEY_ALT_O 0x1800
#define KEY_ALT_P 0x1900
#define KEY_ALT_Q 0x1000
#define KEY_ALT_R 0x1300
#define KEY_ALT_S 0x1F00
#define KEY_ALT_T 0x1400
#define KEY_ALT_U 0x1600
#define KEY_ALT_V 0x2F00
#define KEY_ALT_W 0x1100
#define KEY_ALT_X 0x2D00
#define KEY_ALT_Y 0x1500
#define KEY_ALT_Z 0x2C00
#define KEY_ALT_1 0x7800
#define KEY_ALT_2 0x7900
#define KEY_ALT_3 0x7A00
#define KEY_ALT_4 0x7B00
#define KEY_ALT_5 0x7C00
#define KEY_ALT_6 0x7D00
#define KEY_ALT_7 0x7E00
#define KEY_ALT_8 0x7F00
#define KEY_ALT_9 0x8000
#define KEY_ALT_0 0x8100
#define KEY_ALT_MINUS 0x8200
#define KEY_ALT_EQUAL 0x8300
#define KEY_ALT_SQ_BRACKET_L 0x1A00
#define KEY_ALT_SQ_BRACKET_R 0x1B00
#define KEY_ALT_SEMICOLON 0x2700
#define KEY_ALT_BACKSLASH 0x2600
#define KEY_ALT_F1 0x6800
#define KEY_ALT_F2 0x6900
#define KEY_ALT_F3 0x6A00
#define KEY_ALT_F4 0x6B00
#define KEY_ALT_F5 0x6C00
#define KEY_ALT_F6 0x6D00
#define KEY_ALT_F7 0x6E00
#define KEY_ALT_F8 0x6F00
#define KEY_ALT_F9 0x7000
#define KEY_ALT_F10 0x7100
#define KEY_ALT_F11 0x8B00
#define KEY_ALT_F12 0x8C00
#define KEY_ALT_BACKSPACE 0x0E00
#define KEY_ALT_DELETE 0xA300
#define KEY_ALT_DOWN_ARROW 0xA000
#define KEY_ALT_END 0x9F00
#define KEY_ALT_ENTER 0xA600
#define KEY_ALT_ESC 0x0100
#define KEY_ALT_HOME 0x9700
#define KEY_ALT_INS 0xA200
#define KEY_ALT_KEYPAD_MULTIPLY 0x3700
#define KEY_ALT_KEYPAD_MINUS 0x4A00
#define KEY_ALT_KEYPAD_PLUS 0x4E00
#define KEY_ALT_KEYPAD_DIVIDE 0xA400
#define KEY_ALT_LEFT_ARROW 0x9B00
#define KEY_ALT_PG_DN 0xA100
#define KEY_ALT_PG_UP 0x9900
#define KEY_ALT_RIGHT_ARROW 0x9D00
#define KEY_ALT_SPACE_BAR 0x3920
#define KEY_ALT_TAB 0xA500
#define KEY_ALT_UP_ARROW 0x9800

/** @brief If a key is being pressed, return an int matching that key.
 * Maps to a constant in input.h, may or may not be the character code, and in
 * the case of things like function keys or arrow keys, most certainly is NOT a
 * character code.*/
int input_get_key();

#endif  // !ABSU_INPUT_H