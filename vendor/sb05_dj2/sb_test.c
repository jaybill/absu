#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <math.h>
#include <sys\farptr.h>

#include "sb_lib.h"
#include "sbdetect.h"
#include "sb_misc.h"

void mixingDemo(void);
void modDemo(void);

int wantedRate;
sb_sample *sample[6];
sb_sample *smpl[6];
sb_mod_file *mod;

void main(int argc, char **argv) {
  void mainMenu(void);

  sb_status stat;
  int a;

  clrscr();

  printf("\n\n       Please select a sampling rate.  This can range from 5000 Hz to 45454 Hz,");
  printf("\n\rand should reflect the speed of your computer.  Some examples:\n");
  printf("\n\t\t386 33-40: 11000 Hz\n\t\t486 DX-33: 22050 Hz\n\t\t486 DX2/4: 44100 Hz\n");
  printf("\n(Note that not all sound cards support the full range of sampling rates,\n");
  printf("but if you select a sampling rate higher than your sound card supports, the\n");
  printf("library will select the best rate your card does support.)\n\n");

  printf("Please enter your sampling rate, in Hz: ");
  scanf("%d",&wantedRate);

  if(sb_install_driver(wantedRate)!=SB_SUCCESS) {
    fprintf(stderr,"Driver error: %s",sb_driver_error);
    exit(1);
  }

  sample[0]=sb_load_sample("lala0.raw",_SB_RAW);
  sample[1]=sb_load_sample("lala1.raw",_SB_RAW);
  sample[2]=sb_load_sample("lala2.raw",_SB_RAW);
  sample[3]=sb_load_sample("aha.wav",_SB_WAV);
  sample[4]=sb_load_sample("doink.wav",_SB_WAV);
  sample[5]=sb_load_sample("thunder.wav",_SB_WAV);
  for(a=0;a<6;a++) {
    if(sample[a]==NULL) {
      fprintf(stderr,"Error on sample %d: %s\n\n",a,sb_digi_error);
      sb_uninstall_driver();
      exit(1);
    }
    if(sb_sample_frequency!=11000) {
      cprintf("\n\rConverting sample %d",a);
      smpl[a]=sb_convert_frequency(sample[a],11000,sb_sample_frequency);
      if(smpl[a]!=NULL)
        sb_free_sample(sample[a]);
      else {
        fprintf(stderr,"Error converting sample %d: %s",a,sb_digi_error);
        sb_uninstall_driver();
        exit(1);
      }
    }
    else
      smpl[a]=sample[a];
  }
  if(argc<=1)
    mod=sb_load_mod_file("tune2.mod");
  else
    mod=sb_load_mod_file(argv[1]);
  if(mod==NULL) {
    fprintf(stderr,"MOD Error: %s\n\n",sb_mod_error);
    sb_uninstall_driver();
    exit(1);
  }
  
  mainMenu();
  
  sb_uninstall_driver();
  for(a=0;a<6;a++)
    sb_free_sample(smpl[a]);
  sb_free_mod_file(mod);
}

void mainMenu(void) {
  char k;

  do {
    clrscr();

    if(sb_info.dspVersion>=0x040C)
      cprintf("Sound-Blaster AWE32");
    else if(sb_info.dspVersion>=0x0400)
      cprintf("Sound-Blaster 16");
    else if(sb_info.dspVersion>=0x0300)
      cprintf("Sound-Blaster Pro");
    else if(sb_info.dspVersion>=0x0201)
      cprintf("Sound-Blaster 2.0");
    else
      cprintf("Sound-Blaster 1.0/1.5");

    cprintf(" detected at Address %xH, IRQ %d\n\r",sb_info.reset-6,sb_info.IRQ);
    cprintf("Using a sampling rate of %d",sb_sample_frequency);

    cprintf("\n\n\n\n\n\n\r                Please select feature to demo");
    cprintf("\n\n\r                1> Sample mixing/queuing");
    cprintf("\n\r                2> .MOD player");
    cprintf("\n\n\r                3> Exit");

    do {
      while(!kbhit());
      k=getch();
    } while((k!='1')&&(k!='2')&&(k!='3'));

    if(k=='1')
      mixingDemo();
    else if(k=='2')
      modDemo();

  } while(k!='3');
}

void modDemo(void) {
  char k;
  float fakeVolume=100.0;
  int d;

  if(sb_mod_volume<7) {
    for(d=7;d>sb_mod_volume;d--)
      fakeVolume*=2.0;
  }
  else if(sb_mod_volume>7) {
    for(d=7;d<sb_mod_volume;d++)
      fakeVolume/=2.0;
  }

  clrscr();
  if(!sb_mod_active)
    sb_mod_play(mod);

  gotoxy(1,18);
  cprintf("-------------------------------------------------------------------------------");
  gotoxy(1,19);
  cprintf("P) Pause MOD                  R) Resume MOD");
  gotoxy(1,20);
  cprintf("-) Decrease Volume            +) Increase Volume       (Current Volume: %4.0f%%)  ",fakeVolume);
  gotoxy(1,21);
  cprintf("Q) Go back to main menu");

  do {
    sb_mod_show_info();
    k=getch();
    if(k=='-') {
      if(sb_mod_volume<14) {
        sb_mod_volume++;
        fakeVolume/=2.0;
        gotoxy(1,20);
        cprintf("-) Decrease Volume            +) Increase Volume       (Current Volume: %4.0f%%)",fakeVolume);
      }
    }
    else if(k=='+') {
      if(sb_mod_volume>2) {
        sb_mod_volume--;
        fakeVolume*=2.0;
        gotoxy(1,20);
        cprintf("-) Decrease Volume            +) Increase Volume       (Current Volume: %4.0f%%)",fakeVolume);
      }
    }
    else if(isalpha(k)) {
      k=toupper(k);
      if(k=='P')
        sb_mod_active=0;
      else if(k=='R')
        sb_mod_active=1;
      else if(k=='Q')
        k=27;
    }
  } while(k!=27);
}

void mixingDemo(void) {
  int a;
  int volume[6]={0,0,0,0,0,0};
  int curSample=0;
  char k;

  clrscr();

  gotoxy(9,2);
  cprintf("SAMPLE NAME");
  gotoxy(6,3);
  cprintf("-----------------");
  gotoxy(50,1);
  cprintf("VOLUME");
  gotoxy(44,2);
  cprintf(" LEFT <-----> RIGHT");
  
  gotoxy(6,4);
  cprintf("1> LALA0.RAW");
  gotoxy(6,5);
  cprintf("2> LALA1.RAW");
  gotoxy(6,6);
  cprintf("3> LALA2.RAW");
  gotoxy(6,7);
  cprintf("4> AHA.WAV");
  gotoxy(6,8);
  cprintf("5> DOINK.WAV");
  gotoxy(6,9);
  cprintf("6> THUNDER.WAV");
  
  gotoxy(1,15);
  cprintf("  Press keys 1 - 6 to mix a sample, press <SHIFT>1 - 6 to queue a sample.");
  cprintf("\n\rUse the Up/Down arrow keys to select a sample and the Left/Right arrow");
  cprintf("\n\rkeys to change the volume in the left and right speakers.");
  cprintf("\n\r  Press ESC to exit.");

  do {
    for(a=0;a<6;a++) {
      if(volume[a]<0)
        smpl[a]->right_volume=27-(abs(volume[a])*3);
      else if(volume[a]>0)
        smpl[a]->left_volume=27-(volume[a]*3);
      else {
        smpl[a]->left_volume=32;
        smpl[a]->right_volume=32;
      }
      if(a!=curSample) {
        gotoxy(44,4+a);
        cprintf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
        gotoxy(53+volume[a],4+a);
        cprintf("Å");
      }
    }
    gotoxy(44,4+curSample);
    cprintf("ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ");
    gotoxy(53+volume[curSample],4+curSample);
    cprintf("Î");

    while(!kbhit());
    k=getch();
    if(k==0) {
      switch(getch()) {
        case 80:
          curSample+=1;
          if(curSample>5)
            curSample=5;
          break;
        case 75:
          volume[curSample]--;
          if(volume[curSample]<-9)
            volume[curSample]=-9;
          break;
        case 72:
          curSample-=1;
          if(curSample<0)
            curSample=0;
          break;
        case 77:
          volume[curSample]++;
          if(volume[curSample]>9)
            volume[curSample]=9;
          break;
      }
    }
    else if((k>='1')&&(k<='6'))
      sb_mix_sample(smpl[k-'1']);
    else if(k=='!')
      sb_queue_sample(smpl[0]);
    else if(k=='@')
      sb_queue_sample(smpl[1]);
    else if(k=='#')
      sb_queue_sample(smpl[2]);
    else if(k=='$')
      sb_queue_sample(smpl[3]);
    else if(k=='%')
      sb_queue_sample(smpl[4]);
    else if(k=='^')
      sb_queue_sample(smpl[5]);
  } while(k!=27);
}
