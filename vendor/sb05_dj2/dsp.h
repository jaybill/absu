#ifndef __SB_DSP
#define __SB_DSP
#ifdef __cplusplus
extern "C" {
#endif

/* Pass:                                                                      */
/* Returns: 1 if reset was successful, 0 otherwise.                           */
int sb_dspReset(void);

/* Pass: BYTE to write to the DSP                                             */
/* Returns:                                                                   */
inline void sb_dspWrite(int);

/* Pass:                                                                      */
/* Returns: Status BYTE from the DSP                                          */
inline int sb_dspRead(void);

/* Pass: Frequency (Hz) for DMA transfers.                                    */
/* Returns:                                                                   */
void sb_dspSetTimeConstant(int);

void sb_dspSetHighSpeedTimeConstant(int);

/* Pass: Size of transfer buffer, minus 1 (in BYTES)                          */
/* Returns:                                                                   */
void sb_dspStartDMATransferSC(int);

/* Pass: Size of transfer buffer, minus 1 (in BYTES)                          */
/* Returns:                                                                   */
void sb_dspStartDMATransferAI(int);

void sb_dspStartHighSpeedDMATransferAI(int);

int sb_dsp_module_lock_your_memory(void);

#ifdef __cplusplus
}
#endif
#endif
