#include <inlines/pc.h>

#include "sb_defs.h"
#include "sbdetect.h"
#include "sb_misc.h"

inline void sb_dspWrite(int val) {
  while((inportb(sb_info.writeData)&0x80)!=0);
  outportb(sb_info.writeData,val);
}

inline int sb_dspRead(void) {
  while((inportb(sb_info.dataAvail)&0x80)==0);
  return inportb(sb_info.readData);
}

int sb_dspReset(void) {
  int a;
  int success;

  outportb(sb_info.reset,1);

  sb_microWait(4);

  outportb(sb_info.reset,0);

  success=0;
  for(a=0;a<1000;a++) {
    if(inportb(sb_info.dataAvail)&0x80) {
      success=1;
      break;
    }
  }

  if(success) {
    for(a=0;a<1000;a++) {
      if(inportb(sb_info.readData)==0xAA) {
        success=2;
        break;
      }
    }
  }

  if(success!=2)
    return 0;

  sb_dspWrite(0xE1);
  sb_info.dspVersion=sb_dspRead();
  sb_info.dspVersion<<=8;
  sb_info.dspVersion|=sb_dspRead();

  return 1;
}

void sb_dspSetTimeConstant(int freq) {
  BYTE tc;

  sb_dspWrite(0x40);

  tc=256-(1000000/freq);
  sb_dspWrite(tc);
}

void sb_dspSetHighSpeedTimeConstant(int freq) {
  unsigned tc;

  sb_dspWrite(0x40);

  tc=65536-(256000000/freq);
  tc>>=8;
  sb_dspWrite(tc);
}

void sb_dspStartDMATransferAI(int length) {
  sb_dspWrite(0x48);
  sb_dspWrite(length&0xFF);
  sb_dspWrite(length>>8);
  sb_dspWrite(0x1C);
}

void sb_dspStartHighSpeedDMATransferAI(int length) {
  sb_dspWrite(0x48);
  sb_dspWrite(length&0xFF);
  sb_dspWrite(length>>8);
  sb_dspWrite(0x90);
}

static int topOfFunctions(void) { }

void sb_dspStartDMATransferSC(int length) {
  sb_dspWrite(0x14);
  sb_dspWrite(length&0xFF);
  sb_dspWrite(length>>8);
}

static int bottomOfFunctions(void) { }

int sb_dsp_module_lock_your_memory(void) {
  return !(_go32_dpmi_lock_code(topOfFunctions,((char *)bottomOfFunctions-(char *)topOfFunctions)));
}
