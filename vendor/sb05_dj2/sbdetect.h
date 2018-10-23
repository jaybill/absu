#ifndef __SB_DETECT
#define __SB_DETECT
#ifdef __cplusplus
extern "C" {
#endif

/* All the info I need about the sound-blaster currently installed.           */
extern struct {
  WORD reset;
  WORD readData;
  WORD writeData;
  WORD dataAvail;
  int IRQ;
  int DMA;
  int dspVersion;
} sb_info;

/* Pass:                                                                      */
/* Returns: sb_status enum indicating anything that might have gone wrong.    */
sb_status sb_is_present(void);

/* Pass: New DMA channel. Default is 1, and the sb_is_present() won't find it
         if it's anywhere else.                                               */
/* Returns:                                                                   */
void sb_change_dma_channel(int);

#ifdef __cplusplus
}
#endif
#endif
