#include <inlines/pc.h>

#include "sb_defs.h"
#include "sb_misc.h"
#include "sbdetect.h"

static int pagePort[8] = { 0x87, 0x83, 0x81, 0x82, -1, 0x8B, 0x89, 0x8A };
static BYTE dmaCommand;

static int topOfFunctions(void) { }

static inline programDMA(DWORD linearAddressOfBuffer, int length) {
  int offset, page;
  int port;

  page=linearAddressOfBuffer>>16;
  offset=linearAddressOfBuffer&0xFFFF;

  outportb(0x0A,sb_info.DMA|0x04);              /* Mask DMA channel     */
  outportb(0x0C,0);                             /* Clear byte flip-flop */
  outportb(0x0B,sb_info.DMA|dmaCommand);        /* Set mode             */

  port=sb_info.DMA<<1;
  outportb(port,offset&0xFF);                   /* Program offset       */
  outportb(port,offset>>8);

  port+=1;
  outportb(port,length&0xFF);                   /* Program length       */
  outportb(port,length>>8);

  outportb(pagePort[sb_info.DMA],page);         /* Program page         */

  outportb(0x0A,sb_info.DMA);                   /* Clear channel mask   */
}

void sb_dma8bitReadAI(DWORD linearAddressOfBuffer, int length) {
  dmaCommand=0x58;
  programDMA(linearAddressOfBuffer,length);
}

void sb_dma8bitReadSC(DWORD linearAddressOfBuffer, int length) {
  dmaCommand=0x48;
  programDMA(linearAddressOfBuffer,length);
}

static int bottomOfFunctions(void) { }

int sb_dma_module_lock_your_memory(void) {

  if(!_go32_dpmi_lock_code(topOfFunctions,((char *)bottomOfFunctions-(char *)topOfFunctions)))
    if(!_go32_dpmi_lock_data(pagePort,8*sizeof(int)))
      if(!_go32_dpmi_lock_data(&pagePort,sizeof(int *)))
        return !(_go32_dpmi_lock_data(&dmaCommand,sizeof(BYTE)));

  return 0;
}
