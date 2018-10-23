CC      =gcc
CFLAGS  =-O2 -D_SB_INCLUDE_MOD -D_SB_INCLUDE_FILE_UTILS -D_SB_INCLUDE_CONVERSION_UTILS -D_SB_MOD_SHOW_INFO
SB_OBJ  =dsp.o dma.o sbdetect.o sbdriver.o sb_misc.o sb_digi.o sb_mod.o sb_test.o
RM      =del

sb_test.exe: $(SB_OBJ)
	$(CC) -o sb_test.cof $(SB_OBJ) -lemu
	stubify sb_test.cof

clean:
	$(RM) *.o

dsp.o           : dsp.c sb_defs.h sbdetect.h sb_misc.h
dma.o           : dma.c sb_defs.h sbdetect.h sb_misc.h
sb_misc.o       : sb_misc.c sb_defs.h
sbdetect.o      : sbdetect.c sb_misc.h sb_defs.h dsp.h
sb_digi.o       : sb_digi.c sb_misc.h sb_defs.h
sbdriver.o      : sbdriver.c sb_misc.h sbdetect.h dsp.h dma.h sb_digi.h sb_defs.h
sb_mod.o        : sb_mod.c sb_defs.h sb_misc.h sb_digi.h sbdriver.h
sb_test.o       : sb_test.c sbdetect.h sb_misc.h sb_digi.h sbdriver.h sb_defs.h sb_mod.h
