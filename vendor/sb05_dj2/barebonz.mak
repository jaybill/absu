CC      =gcc
CFLAGS  =-O2
SB_OBJ  =dsp.o dma.o sbdetect.o sbdriver.o sb_misc.o sb_digi.o
RM      =del

libsb.a: $(SB_OBJ)
	       ar cq libsb.a $(SB_OBJ)
	       ranlib libsb.a

clean:
	       $(RM) *.o
	       $(RM) libsb.a

dsp.o           : dsp.c sb_defs.h sbdetect.h sb_misc.h
dma.o           : dma.c sb_defs.h sbdetect.h sb_misc.h
sb_misc.o       : sb_misc.c sb_defs.h
sbdetect.o      : sbdetect.c sb_misc.h sb_defs.h dsp.h
sb_digi.o       : sb_digi.c sb_misc.h sb_defs.h
sbdriver.o      : sbdriver.c sb_misc.h sbdetect.h dsp.h dma.h sb_digi.h sb_defs.h
