#include <pc.h>
#include <go32.h>
#include <dpmi.h>
#include <string.h>
#include <stdlib.h>
#include <sys/exceptn.h>

#include "sb_defs.h"

void sb_waitInit(void) {
  int temp;

  temp=inportb(0x61);
  temp&=0xFD;
  temp|=0x01;
  outportb(0x61,temp);
}

void sb_microWait(WORD usec) {
  WORD elapsed;
  unsigned long failsafe;

  outportb(0x43,0xB0);
  outportb(0x42,0xFF);
  outportb(0x42,0xFF);

  /* Sometimes this timer doesn't seem to work, and our program hangs! */
  failsafe = usec * 10000;

  do {
    outportb(0x43,0x80);
    elapsed=inportb(0x42);
    elapsed|=(inportb(0x42)<<8);
    elapsed=~elapsed;
  } while(elapsed<usec && failsafe--);
}
