#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dpmi.h>
#include <go32.h>
#include <inlines/pc.h>

#include "sb_defs.h"
#include "sb_misc.h"
#include "dsp.h"

struct {                        /* This holds the vital info on the Sound- */
  WORD reset;                   /* Blaster, like where it's located, which */
  WORD readData;                /* IRQ it uses, and which DSP it had.      */
  WORD writeData;
  WORD dataAvail;
  int IRQ;
  int DMA;
  int dspVersion;
} sb_info;

static volatile int testCount;
static int dontSetDMA=0;

/*//////////////////////////////////////////////////////////////////////////////
/                        Functions for this module only                        /
//////////////////////////////////////////////////////////////////////////////*/

static int topOfFunctions(void) { }

static void testInt2(void) {
  testCount=2;
  inportb(sb_info.dataAvail);
  outportb(0x20,0x20);
}

static void testInt3(void) {
  testCount=3;
  inportb(sb_info.dataAvail);
  outportb(0x20,0x20);
}

static void testInt5(void) {
  testCount=5;
  inportb(sb_info.dataAvail);
  outportb(0x20,0x20);
}

static void testInt7(void) {
  testCount=7;
  inportb(sb_info.dataAvail);
  outportb(0x20,0x20);
}

static void testInt10(void) {
  testCount=10;
  inportb(sb_info.dataAvail);
  outportb(0x20,0x20);
  outportb(0xA0,0x20);
}

static int bottomOfFunctions(void) { }

static int findInterrupt(void) {
  __dpmi_paddr old2, old3, old5, old7, old10;
  __dpmi_paddr new2, new3, new5, new7, new10;
  _go32_dpmi_seginfo wrap2, wrap3, wrap5, wrap7, wrap10;
  BYTE pic1Default, pic2Default;

  testCount=0;

  if(!_go32_dpmi_lock_code(topOfFunctions,((char *)bottomOfFunctions-(char *)topOfFunctions))) {
    if(_go32_dpmi_lock_data(&testCount,sizeof(int)))
      return 0;
  }
  else
    return 0;

  __dpmi_get_protected_mode_interrupt_vector(0x0A,&old2);
  __dpmi_get_protected_mode_interrupt_vector(0x0B,&old3);
  __dpmi_get_protected_mode_interrupt_vector(0x0D,&old5);
  __dpmi_get_protected_mode_interrupt_vector(0x0F,&old7);
  __dpmi_get_protected_mode_interrupt_vector(0x72,&old10);

  wrap2.pm_offset=(int)testInt2;
  wrap2.pm_selector=_my_cs();
  wrap3.pm_offset=(int)testInt3;
  wrap3.pm_selector=_my_cs();
  wrap5.pm_offset=(int)testInt5;
  wrap5.pm_selector=_my_cs();
  wrap7.pm_offset=(int)testInt7;
  wrap7.pm_selector=_my_cs();
  wrap10.pm_offset=(int)testInt10;
  wrap10.pm_selector=_my_cs();

  _go32_dpmi_allocate_iret_wrapper(&wrap2);
  _go32_dpmi_allocate_iret_wrapper(&wrap3);
  _go32_dpmi_allocate_iret_wrapper(&wrap5);
  _go32_dpmi_allocate_iret_wrapper(&wrap7);
  _go32_dpmi_allocate_iret_wrapper(&wrap10);

  new2.offset32=wrap2.pm_offset;
  new2.selector=wrap2.pm_selector;
  new3.offset32=wrap3.pm_offset;
  new3.selector=wrap3.pm_selector;
  new5.offset32=wrap5.pm_offset;
  new5.selector=wrap5.pm_selector;
  new7.offset32=wrap7.pm_offset;
  new7.selector=wrap7.pm_selector;
  new10.offset32=wrap10.pm_offset;
  new10.selector=wrap10.pm_selector;

  pic1Default=inportb(0x21);
  pic2Default=inportb(0xA1);

  outportb(0x21,pic1Default&0x53);      /* Clear all relevent masks */
  outportb(0xA1,pic2Default&0xFB);
  __dpmi_set_protected_mode_interrupt_vector(0x0A,&new2);
  __dpmi_set_protected_mode_interrupt_vector(0x0B,&new3);
  __dpmi_set_protected_mode_interrupt_vector(0x0D,&new5);
  __dpmi_set_protected_mode_interrupt_vector(0x0F,&new7);
  __dpmi_set_protected_mode_interrupt_vector(0x72,&new10);

  sb_dspWrite(0xF2);                    /* This will force the DSP to signal */
  while(testCount==0);                  /* a hardware interrupt, which one of */
                                        /* the functions I set up will handle. */
  outportb(0x21,pic1Default);
  outportb(0xA1,pic2Default);
  __dpmi_set_protected_mode_interrupt_vector(0x0A,&old2);
  __dpmi_set_protected_mode_interrupt_vector(0x0B,&old3);
  __dpmi_set_protected_mode_interrupt_vector(0x0D,&old5);
  __dpmi_set_protected_mode_interrupt_vector(0x0F,&old7);
  __dpmi_set_protected_mode_interrupt_vector(0x72,&old10);

  _go32_dpmi_free_iret_wrapper(&wrap2);
  _go32_dpmi_free_iret_wrapper(&wrap3);
  _go32_dpmi_free_iret_wrapper(&wrap5);
  _go32_dpmi_free_iret_wrapper(&wrap7);
  _go32_dpmi_free_iret_wrapper(&wrap10);

  return testCount;
} 

static sb_status findSoundBlaster(void) {
  static WORD ioaddr[7] = { 0x220, 0x240, 0x210, 0x230, 0x250, 0x260, 0x280 };
  int a;
  sb_status stat=SB_FAILURE;

  for(a=0;a<7;a++) {
    sb_info.reset=ioaddr[a]+0x06;
    sb_info.readData=ioaddr[a]+0x0A;
    sb_info.writeData=ioaddr[a]+0x0C;
    sb_info.dataAvail=ioaddr[a]+0x0E;

    if(sb_dspReset()) {                 /* Found the right IO address! */
      a=7;
      if(sb_info.IRQ=findInterrupt()) { /* ...grab the interrupt vector */
        if(!dontSetDMA)
          sb_info.DMA=1;                  /* Assume DMA channel 1, because there */
        stat=SB_SUCCESS;                  /* is no reliable way to find it.  */
      }
    }
  }
  return stat;
}

/*//////////////////////////////////////////////////////////////////////////////
/                              Global functions                                /
//////////////////////////////////////////////////////////////////////////////*/

sb_status sb_is_present(void) {
  static char *blaster;
  char *address;
  short sbIO;
  sb_status temp;

  sb_waitInit();
  blaster=getenv("BLASTER");

  if(_go32_dpmi_lock_data(&sb_info,sizeof(sb_info)))
    return SB_FAILURE;

  if(blaster!=NULL) {
    strupr(blaster);
    address=strrchr(blaster,'A');
    if(address!=NULL) {
      ++address;
      sscanf(address,"%hx",&sbIO);
      if((sbIO==0x210)||(sbIO==0x220)||(sbIO==0x230)||(sbIO==0x240)||(sbIO==0x250)||(sbIO==0x260)||(sbIO==0x280)) {
        sb_info.reset=sbIO+0x06;
        sb_info.readData=sbIO+0x0A;
        sb_info.writeData=sbIO+0x0C;
        sb_info.dataAvail=sbIO+0x0E;

        address=strrchr(blaster,'I');
        if(address!=NULL) {
          ++address;
          sscanf(address,"%d",&sb_info.IRQ);
          if((sb_info.IRQ==2)||(sb_info.IRQ==3)||(sb_info.IRQ==5)||(sb_info.IRQ==7)||(sb_info.IRQ==10)) {

            address=strrchr(blaster,'D');
            if(address!=NULL) {
              ++address;
              sscanf(address,"%d",&sb_info.DMA);
              if((sb_info.DMA==0)||(sb_info.DMA==1)||(sb_info.DMA==3)) {
                if(sb_dspReset())                       /* Verify address */
                  if(sb_info.IRQ==findInterrupt())      /* Verify IRQ setting */
                    temp=SB_SUCCESS;
                  else
                    temp=SB_BAD_IRQ;
                else
                  temp=SB_BAD_ADDRESS;
              }
              else
                temp=SB_BAD_DMA;
            }
            else
              temp=SB_BAD_BLASTER;
          }
          else
            temp=SB_BAD_IRQ;
        }
        else
          temp=SB_BAD_BLASTER;
      }
      else
        temp=SB_BAD_ADDRESS;
    }
    else
      temp=SB_BAD_BLASTER;
  }
  else {
    temp=findSoundBlaster();
  }

  return temp;
}

void sb_change_dma_channel(int newChannel) {
  sb_info.DMA=newChannel;
  dontSetDMA=1;
}
