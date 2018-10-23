#include <dpmi.h>
#include <go32.h>
#include <inlines/pc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/farptr.h>

#include "dma.h"
#include "dsp.h"
#include "sb_defs.h"
#include "sb_digi.h"
#include "sb_misc.h"
#include "sbdetect.h"

#ifdef _SB_INCLUDE_MOD
#include "sb_mod.h"
#endif

#define _SB_HAS_HIGH_SPEED_MODE (sb_info.dspVersion > 0x0202)

char sb_driver_error[80];

DWORD sb_dmaBufferLinearAddress[2];
DWORD sb_miniDMABufferAddress[2];
int sb_currentBlock;
int sb_sample_frequency;

static BYTE mixerDefault;
static BYTE pic1Default, pic2Default;
static WORD dmaBufferSelector;
static int endOfDMAInterruptVector;
static __dpmi_paddr oldHandler, newHandler;
static _go32_dpmi_seginfo wrapper;

static int topOfFunctions(void) {}

static void sb_interrupt_handler(void) {
  int a, b;
  DWORD addr;

  asm("movl $96, %%ecx\n\t"
      "movl $0x80, %%eax\n\t"
      "movl %0, %%edi\n\t"
      "cld\n\t"
      "rep\n\t"
      "stosl"
      :
      : "m"(sb_mixing_buffer)
      : "%eax", "%ecx", "%edi");

  if (sb_info.dspVersion >= 0x0300) { /* Use STEREO */
#ifdef _SB_INCLUDE_MOD
    if (sb_mod_active) sb_mod_parse_file();
#endif
    sb_mix_stereo_samples();
    _farsetsel(_go32_info_block.selector_for_linear_memory);
    addr = sb_dmaBufferLinearAddress[sb_currentBlock];
    for (a = 0; a < (_SB_BLOCK_SIZE / 2); a++, addr += 2) {
      if (sb_left_buffer[a] < 0)
        _farnspokeb(addr, (BYTE)0);
      else if (sb_left_buffer[a] > 255)
        _farnspokeb(addr, (BYTE)255);
      else
        _farnspokeb(addr, (BYTE)sb_left_buffer[a]);
    }
    addr -= 47;
    for (a = 0; a < (_SB_BLOCK_SIZE / 2); a++, addr += 2) {
      if (sb_right_buffer[a] < 0)
        _farnspokeb(addr, (BYTE)0);
      else if (sb_right_buffer[a] > 255)
        _farnspokeb(addr, (BYTE)255);
      else
        _farnspokeb(addr, (BYTE)sb_right_buffer[a]);
    }
    sb_currentBlock ^= 1;
  } else { /* Use MONO (Blech!) */
#ifdef _SB_INCLUDE_MOD
    if (sb_mod_active) sb_mod_parse_file_mono();
#endif
    sb_mix_mono_samples();
    addr = sb_miniDMABufferAddress[sb_currentBlock];
    _farsetsel(_go32_info_block.selector_for_linear_memory);
    for (a = 0; a < (_SB_BLOCK_SIZE / 2); a++, addr++) {
      if (sb_mixing_buffer[a] < 0)
        _farnspokeb(addr, (BYTE)0);
      else if (sb_mixing_buffer[a] > 255)
        _farnspokeb(addr, (BYTE)255);
      else
        _farnspokeb(addr, (BYTE)sb_mixing_buffer[a]);
    }
    sb_currentBlock ^= 1;
  }

  if (sb_info.dspVersion < 0x0200) { /* Ancient SB? */
    sb_dma8bitReadSC(sb_miniDMABufferAddress[sb_currentBlock],
                     (_SB_BLOCK_SIZE / 2) - 1);
    sb_dspStartDMATransferSC((_SB_BLOCK_SIZE / 2) - 1);
  }

  inportb(sb_info.dataAvail);
  outportb(0x20, 0x20);
  outportb(0xA0, 0x20);
}

static int bottomOfFunctions(void) {}

/* Allocate some DOS memory that doesn't cross a PAGE boundary (for the DMA
   buffer.)                                                                   */
static int allocateDosMem(int bytes, WORD *dosSeg, WORD *dosSel) {
  int firstPage, lastPage;
  int linearAddress;
  int dosSegs[16];
  int dosSels[16];
  int paragraphs = (bytes + 15) >> 4;
  int currentTry = -1;

  do {
    ++currentTry;

    if (currentTry > 15) return 0;

    dosSegs[currentTry] =
        __dpmi_allocate_dos_memory(paragraphs, &dosSels[currentTry]);

    if (dosSegs[currentTry] == -1) return 0;

    linearAddress = dosSegs[currentTry] << 4;
    firstPage = linearAddress >> 16;
    lastPage = (linearAddress + bytes - 1) >> 16;

  } while (firstPage != lastPage);

  *dosSeg = (WORD)dosSegs[currentTry];
  *dosSel = (WORD)dosSels[currentTry];

  for (currentTry -= 1; currentTry >= 0; currentTry--)
    __dpmi_free_dos_memory(dosSels[currentTry]);

  return 1;
}

sb_status sb_install_driver(int frequency) {
  sb_status stat;
  WORD dmaBufferSegment;
  BYTE picMask;

  if ((stat = sb_is_present()) == SB_SUCCESS) {
    if (!allocateDosMem(_SB_BUFFER_SIZE, &dmaBufferSegment,
                        &dmaBufferSelector)) {
      stat = SB_FAILURE;
      strcpy(sb_driver_error, "Unable to allocate DOS memory buffer!");
      return stat;
    }
    sb_dmaBufferLinearAddress[0] = (DWORD)dmaBufferSegment << 4;
    sb_dmaBufferLinearAddress[1] =
        sb_dmaBufferLinearAddress[0] + _SB_BLOCK_SIZE;
    sb_miniDMABufferAddress[0] = sb_dmaBufferLinearAddress[0];
    sb_miniDMABufferAddress[1] =
        sb_dmaBufferLinearAddress[0] + (_SB_BLOCK_SIZE / 2);

    pic1Default = inportb(0x21);
    pic2Default = inportb(0xA1);

    if (sb_info.IRQ < 8) {
      endOfDMAInterruptVector = sb_info.IRQ + 0x08;
      picMask = 1 << sb_info.IRQ;
      picMask = ~picMask;
      outportb(0x21, pic1Default & picMask); /* Enable PIC-1's IRQ */
    } else {
      endOfDMAInterruptVector = sb_info.IRQ + 0x68;
      picMask = 1 << (sb_info.IRQ - 8);
      picMask = ~picMask;
      outportb(0x21, pic1Default & 0xFB);    /* Enable IRQ2 */
      outportb(0xA1, pic2Default & picMask); /* As well as PIC-2's IRQ */
    }

    wrapper.pm_offset = (int)sb_interrupt_handler;
    wrapper.pm_selector = _my_cs();
    _go32_dpmi_allocate_iret_wrapper(&wrapper);
    newHandler.offset32 = wrapper.pm_offset;
    newHandler.selector = wrapper.pm_selector;

    if ((_go32_dpmi_lock_code(topOfFunctions, ((char *)bottomOfFunctions -
                                               (char *)topOfFunctions))) ||
        (_go32_dpmi_lock_data(&wrapper, sizeof(_go32_dpmi_seginfo))) ||
        (_go32_dpmi_lock_data(sb_dmaBufferLinearAddress, 2 * sizeof(DWORD))) ||
        (_go32_dpmi_lock_data(&sb_dmaBufferLinearAddress, sizeof(DWORD *))) ||
        (_go32_dpmi_lock_data(sb_miniDMABufferAddress, 2 * sizeof(DWORD))) ||
        (_go32_dpmi_lock_data(&sb_miniDMABufferAddress, sizeof(DWORD *))) ||
        (_go32_dpmi_lock_data(&sb_currentBlock, sizeof(int))) ||
        (_go32_dpmi_lock_data(&sb_sample_frequency, sizeof(int))) ||
        (!sb_dma_module_lock_your_memory()) ||
        (!sb_dsp_module_lock_your_memory()) ||
#ifdef _SB_INCLUDE_MODE
        (!sb_mod_module_lock_your_memory()) ||
#endif
        (!sb_digi_module_lock_your_memory())) {
      strcpy(sb_driver_error, "Unable to lock appropriate memory.");
      return SB_FAILURE;
    }

    sb_currentBlock = 0;
    sb_sample_frequency = frequency;

    if (sb_sample_frequency < 5000) sb_sample_frequency = 5000;

    if (sb_info.dspVersion >= 0x0400) {
      if (sb_sample_frequency > 45454) sb_sample_frequency = 45454;
    } else if (sb_info.dspVersion >= 0x0300) {
      if (sb_sample_frequency > 22727) sb_sample_frequency = 22727;
    } else if (_SB_HAS_HIGH_SPEED_MODE) {
      if (sb_sample_frequency > 45454) sb_sample_frequency = 45454;
    } else {
      if (sb_sample_frequency > 22222) sb_sample_frequency = 22222;
    }

    __dpmi_get_protected_mode_interrupt_vector(endOfDMAInterruptVector,
                                               &oldHandler);
    __dpmi_set_protected_mode_interrupt_vector(endOfDMAInterruptVector,
                                               &newHandler);

    if (_SB_HAS_HIGH_SPEED_MODE) { /* Use High-Speed Mode */
      if ((sb_info.dspVersion >= 0x0300) && (sb_info.dspVersion < 0x0400))
        sb_dspSetHighSpeedTimeConstant(sb_sample_frequency * 2);
      else
        sb_dspSetHighSpeedTimeConstant(sb_sample_frequency);
    } else /* Use normal mode */
      sb_dspSetTimeConstant(sb_sample_frequency);

    sb_digi_initialize();
#ifdef _SB_INCLUDE_MOD
    sb_mod_initialize_player();
#endif

    sb_dspWrite(0xD1); /* Turn the speaker on */

    if (sb_info.dspVersion >= 0x0400) { /* SB16 or better */
      sb_dma8bitReadAI(sb_dmaBufferLinearAddress[0], _SB_BUFFER_SIZE - 1);
      sb_dspWrite(0xC4);
      sb_dspWrite(0x20);
      sb_dspWrite((_SB_BLOCK_SIZE - 1) & 0xFF);
      sb_dspWrite((_SB_BLOCK_SIZE - 1) / 256);
    } else if (sb_info.dspVersion >= 0x0300) { /* SBPro */
      outportb(sb_info.reset - 2, 0x0E);
      mixerDefault = inportb(sb_info.reset - 1);
      outportb(sb_info.reset - 2, 0x0E);
      outportb(sb_info.reset - 1, mixerDefault | 0x22);
      sb_dma8bitReadAI(sb_dmaBufferLinearAddress[0], _SB_BUFFER_SIZE - 1);
      sb_dspStartHighSpeedDMATransferAI(_SB_BLOCK_SIZE - 1);
    } else if (_SB_HAS_HIGH_SPEED_MODE) { /* SB 2.0 */
      sb_dma8bitReadAI(sb_dmaBufferLinearAddress[0], _SB_BUFFER_SIZE - 1);
      sb_dspStartHighSpeedDMATransferAI(_SB_BLOCK_SIZE - 1);
    } else if (sb_info.dspVersion >= 0x0200) { /* SB 1.5 */
      sb_dma8bitReadAI(sb_miniDMABufferAddress[0], _SB_BLOCK_SIZE - 1);
      sb_dspStartDMATransferAI((_SB_BLOCK_SIZE / 2) - 1);
    } else { /* SB 1.0 (VERY OLD!) */
      sb_dma8bitReadSC(sb_miniDMABufferAddress[0], (_SB_BLOCK_SIZE / 2) - 1);
      sb_dspStartDMATransferSC((_SB_BLOCK_SIZE / 2) - 1);
    }
  } else if (stat == SB_BAD_BLASTER)
    strcpy(sb_driver_error, "BLASTER environment variable incomplete!");
  else if (stat == SB_BAD_ADDRESS)
    strcpy(sb_driver_error,
           "Incorrect address specified in BLASTER environment variable!");
  else if (stat == SB_BAD_IRQ)
    strcpy(sb_driver_error,
           "Incorrect IRQ specified in BLASTER environment variable!");
  else if (stat == SB_BAD_DMA)
    strcpy(sb_driver_error,
           "Incorrect DMA channel specified in BLASTER environment variable!");
  else if (stat == SB_FAILURE)
    strcpy(sb_driver_error, "General failure/No sound card detected.");

  return stat;
}

void sb_uninstall_driver(void) {
#ifdef _SB_INCLUDE_MOD
  sb_mod_pause();
#endif
  if (!_SB_HAS_HIGH_SPEED_MODE) {
    if (sb_info.dspVersion == 0x0200) sb_dspWrite(0xDA);
    sb_dspWrite(0xD0); /* Stop any DMA transfers currently going on */
  }
  sb_dspReset();     /* Hard-reset the DSP */
  sb_dspWrite(0xD3); /* and turn its speaker-output off */

  __dpmi_set_protected_mode_interrupt_vector(endOfDMAInterruptVector,
                                             &oldHandler);

  outportb(0x21, pic1Default);
  outportb(0xA1, pic2Default);

  if ((sb_info.dspVersion >= 0x0300) && (sb_info.dspVersion < 0x0400)) {
    outportb(sb_info.reset - 2, 0x0E);
    outportb(sb_info.reset - 1, mixerDefault);
  }

  _go32_dpmi_free_iret_wrapper(&wrapper);
  __dpmi_free_dos_memory(dmaBufferSelector);
  free(sb_mixing_buffer);
}
