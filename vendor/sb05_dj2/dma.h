#ifndef __SB_DMA
#define __SB_DMA
#ifdef __cplusplus
extern "C" {
#endif

/* Pass: Linear address of transfer buffer (DWORD);
         size of transfer buffer, minus 1 (in BYTES)                          */
/* Returns:                                                                   */
void sb_dma8bitReadSC(unsigned long, int);

/* Pass: Linear address of transfer buffer (DWORD);
         size of transfer buffer, minus 1 (in BYTES)                          */
/* Returns:                                                                   */
void sb_dma8bitReadAI(unsigned long, int);

int sb_dma_module_lock_your_memory(void);

#ifdef __cplusplus
}
#endif
#endif
