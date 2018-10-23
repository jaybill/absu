#ifndef __SB_MISC
#define __SB_MISC
#include <dpmi.h>
#ifdef __cplusplus
extern "C" {
#endif

/* This function pauses <number_passed>*0.8381 microseconds.  It simply waits
   for <number_passed> ticks to elapse in the PIT, which runs at slightly over
   1 MHz.  The minimum amount of time this function will wait is probably
   around 4-6 microseconds, but beyond that it's pretty accurate.             */
/* Pass: number of ticks to wait                                          */
/* Returns:                                                               */
void sb_microWait(WORD);

/* This initializes PIT counter 2 for use in our micro-second delay function. */
void sb_waitInit(void);

#ifdef __cplusplus
}
#endif
#endif
