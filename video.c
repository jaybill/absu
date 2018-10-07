///
/// J. William McCarthy ("AUTHOR") CONFIDENTIAL
/// Unpublished Copyright (c) 2017-2018 J. William McCarthy, All Rights
/// Reserved.
///
/// NOTICE:  All information contained herein is, and remains the property of
/// AUTHOR. The intellectual and technical concepts contained herein are
/// proprietary to AUTHOR and may be covered by U.S. and Foreign Patents,
/// patents in process, and are protected by trade secret or copyright law.
/// Dissemination of this information or reproduction of this material is
/// strictly forbidden unless prior written permission is obtained from AUTHOR.
/// Access to the source code contained herein is hereby forbidden to anyone
/// except current AUTHOR employees, managers or contractors who have executed
/// Confidentiality and Non-disclosure agreements explicitly covering such
/// access.
///
/// The copyright notice above does not evidence any actual or intended
/// publication or disclosure  of  this source code, which includes information
/// that is confidential and/or proprietary, and is a trade secret, of  AUTHOR.
/// ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC  PERFORMANCE, OR PUBLIC
/// DISPLAY OF OR THROUGH USE  OF THIS  SOURCE CODE  WITHOUT  THE EXPRESS
/// WRITTEN CONSENT OF AUTHOR IS STRICTLY PROHIBITED, AND IN VIOLATION OF
/// APPLICABLE LAWS AND INTERNATIONAL TREATIES.  THE RECEIPT OR POSSESSION OF
/// THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY
/// RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE,
/// USE, OR SELL ANYTHING THAT IT  MAY DESCRIBE, IN WHOLE OR IN PART.
///

/* video.c */
#include "video.h"

#include <dpmi.h>
#include <go32.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>
#include <sys/farptr.h>

__dpmi_regs regs;

typedef struct VESA_INFO {
  unsigned char VESASignature[4] __attribute__((packed));
  unsigned short VESAVersion __attribute__((packed));
  unsigned long OEMStringPtr __attribute__((packed));
  unsigned char Capabilities[4] __attribute__((packed));
  unsigned long VideoModePtr __attribute__((packed));
  unsigned short TotalMemory __attribute__((packed));
  unsigned short OemSoftwareRev __attribute__((packed));
  unsigned long OemVendorNamePtr __attribute__((packed));
  unsigned long OemProductNamePtr __attribute__((packed));
  unsigned long OemProductRevPtr __attribute__((packed));
  unsigned char Reserved[222] __attribute__((packed));
  unsigned char OemData[256] __attribute__((packed));
} VESA_INFO;

typedef struct MODE_INFO {
  unsigned short ModeAttributes __attribute__((packed));
  unsigned char WinAAttributes __attribute__((packed));
  unsigned char WinBAttributes __attribute__((packed));
  unsigned short WinGranularity __attribute__((packed));
  unsigned short WinSize __attribute__((packed));
  unsigned short WinASegment __attribute__((packed));
  unsigned short WinBSegment __attribute__((packed));
  unsigned long WinFuncPtr __attribute__((packed));
  unsigned short BytesPerScanLine __attribute__((packed));
  unsigned short XResolution __attribute__((packed));
  unsigned short YResolution __attribute__((packed));
  unsigned char XCharSize __attribute__((packed));
  unsigned char YCharSize __attribute__((packed));
  unsigned char NumberOfPlanes __attribute__((packed));
  unsigned char BitsPerPixel __attribute__((packed));
  unsigned char NumberOfBanks __attribute__((packed));
  unsigned char MemoryModel __attribute__((packed));
  unsigned char BankSize __attribute__((packed));
  unsigned char NumberOfImagePages __attribute__((packed));
  unsigned char Reserved_page __attribute__((packed));
  unsigned char RedMaskSize __attribute__((packed));
  unsigned char RedMaskPos __attribute__((packed));
  unsigned char GreenMaskSize __attribute__((packed));
  unsigned char GreenMaskPos __attribute__((packed));
  unsigned char BlueMaskSize __attribute__((packed));
  unsigned char BlueMaskPos __attribute__((packed));
  unsigned char ReservedMaskSize __attribute__((packed));
  unsigned char ReservedMaskPos __attribute__((packed));
  unsigned char DirectColorModeInfo __attribute__((packed));
  unsigned long PhysBasePtr __attribute__((packed));
  unsigned long OffScreenMemOffset __attribute__((packed));
  unsigned short OffScreenMemSize __attribute__((packed));
  unsigned char Reserved[206] __attribute__((packed));
} MODE_INFO;

VESA_INFO vesa_info;
MODE_INFO mode_info;

uint8_t* vid_mem;
uint8_t* back_buffer;

unsigned short scr_w, scr_h;
unsigned char bpp, pixel_stride;
int pitch;

void video_off() { video_set_mode(TEXT_MODE); }

int video_init(unsigned short screen_width, unsigned short screen_height) {
  scr_w = screen_width;
  scr_h = screen_height;

  int err = video_set_vesa_mode(screen_width, screen_height);
  if (err == -1) {
    return -1;
  }
  bpp = mode_info.BitsPerPixel;
  /* Convert bits per pixel into bytes per pixel. Take care of 15-bit modes as
   * well */

  pixel_stride = (bpp | 7) >> 3;
  /* The pitch is the amount of bytes between the start of each row. This isn't
   * always bytes * width. */
  /* This should work for the basic 16 and 32 bpp modes (but not 24) */
  pitch = screen_width * pixel_stride;

  vid_mem = ((char*)VIDEO_MEMORY_START);
  back_buffer = ((char*)(malloc(screen_height * pitch)));
  return 0;
}

int video_set_mode(int mode) {
  memset(&regs, 0, sizeof regs);
  regs.x.ax = mode;
  return __dpmi_int(SET_MODE, &regs);
}

void video_set_vesa_bank(int bank_number) {
  regs.x.ax = 0x4F05;
  regs.x.bx = 0;
  regs.x.dx = bank_number;
  __dpmi_int(0x10, &regs);
}

void video_put_pixel(int x, int y, int color) {
  int address = y * scr_w + x;
  int bank_size = mode_info.WinGranularity * 1024;
  int bank_number = address / bank_size;
  int bank_offset = address % bank_size;

  video_set_vesa_bank(bank_number);

  _farpokeb(_dos_ds, VIDEO_MEMORY_START + bank_offset, color);
}

int video_get_vesa_info() {
  long dosbuf;
  int c;

  /* use the conventional memory transfer buffer */
  dosbuf = __tb & 0xFFFFF;

  /* initialize the buffer to zero */
  for (c = 0; c < sizeof(VESA_INFO); c++) _farpokeb(_dos_ds, dosbuf + c, 0);

  dosmemput("VBE2", 4, dosbuf);

  /* call the VESA function */
  regs.x.ax = 0x4F00;
  regs.x.di = dosbuf & 0xF;
  regs.x.es = (dosbuf >> 4) & 0xFFFF;
  __dpmi_int(0x10, &regs);

  /* quit if there was an error */
  if (regs.h.ah) return -1;

  /* copy the resulting data into our structure */
  dosmemget(dosbuf, sizeof(VESA_INFO), &vesa_info);

  /* check that we got the right magic marker value */
  if (strncmp(vesa_info.VESASignature, "VESA", 4) != 0) return -1;

  /* it worked! */
  return 0;
}

int video_get_mode_info(int mode) {
  __dpmi_regs r;
  long dosbuf;
  int c;

  /* use the conventional memory transfer buffer */
  dosbuf = __tb & 0xFFFFF;

  /* initialize the buffer to zero */
  for (c = 0; c < sizeof(MODE_INFO); c++) _farpokeb(_dos_ds, dosbuf + c, 0);

  /* call the VESA function */
  r.x.ax = 0x4F01;
  r.x.di = dosbuf & 0xF;
  r.x.es = (dosbuf >> 4) & 0xFFFF;
  r.x.cx = mode;
  __dpmi_int(0x10, &r);

  /* quit if there was an error */
  if (r.h.ah) return -1;

  /* copy the resulting data into our structure */
  dosmemget(dosbuf, sizeof(MODE_INFO), &mode_info);

  /* it worked! */
  return 0;
}

int video_find_vesa_mode(int w, int h) {
  int mode_list[256];
  int number_of_modes;
  long mode_ptr;
  int c;

  /* check that the VESA driver exists, and get information about it */
  if (video_get_vesa_info() != 0) return 0;

  /* convert the mode list pointer from seg:offset to a linear address */
  mode_ptr = ((vesa_info.VideoModePtr & 0xFFFF0000) >> 12) +
             (vesa_info.VideoModePtr & 0xFFFF);

  number_of_modes = 0;

  /* read the list of available modes */
  while (_farpeekw(_dos_ds, mode_ptr) != 0xFFFF) {
    mode_list[number_of_modes] = _farpeekw(_dos_ds, mode_ptr);
    number_of_modes++;
    mode_ptr += 2;
  }

  /* scan through the list of modes looking for the one that we want */
  for (c = 0; c < number_of_modes; c++) {
    /* get information about this mode */
    if (video_get_mode_info(mode_list[c]) != 0) continue;

    /* check the flags field to make sure this is a color graphics mode,
     * and that it is supported by the current hardware */
    if ((mode_info.ModeAttributes & 0x19) != 0x19) continue;

    /* check that this mode is the right size */
    if ((mode_info.XResolution != w) || (mode_info.YResolution != h)) continue;

    /* check that there is only one color plane */
    if (mode_info.NumberOfPlanes != 1) continue;

    /* check that it is a packed-pixel mode (other values are used for
     * different memory layouts, eg. 6 for a truecolor resolution) */
    if (mode_info.MemoryModel != 4) continue;

    /* check that this is an 8-bit (256 color) mode */
    if (mode_info.BitsPerPixel != 8) continue;

    /* if it passed all those checks, this must be the mode we want! */
    return mode_list[c];
  }

  /* oh dear, there was no mode matching the one we wanted! */
  return 0;
}

int video_set_vesa_mode(int w, int h) {
  __dpmi_regs r;
  int mode_number;

  /* find the number for this mode */
  mode_number = video_find_vesa_mode(w, h);
  if (!mode_number) return -1;

  /* call the VESA mode set function */
  r.x.ax = 0x4F02;
  r.x.bx = mode_number;
  __dpmi_int(0x10, &r);
  if (r.h.ah) return -1;

  /* it worked! */
  return 0;
}