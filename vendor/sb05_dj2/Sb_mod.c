#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inlines/pc.h>
#include <go32.h>
#include <dpmi.h>

#include "sb_defs.h"
#include "sb_misc.h"
#include "sb_digi.h"
#include "sbdriver.h"

#define MAX_CHANNELS    8

#define NONE                    -1
#define ARPEGGIO                0
#define PORTA_UP                1
#define PORTA_DOWN              2
#define PORTA_TO_NOTE           3
#define CUT_NOTE                4
#define VOLUME_SLIDE            5
#define VIBRATO                 6
#define PORTA_VOLUME_SLIDE      7
#define VIBRATO_VOLUME_SLIDE    8
#define TREMOLO                 9
#define RETRIG_NOTE            10

struct channelInfo {
  int sample;
  char *sampleData;
  DWORD samplePosition;
  WORD *sampleIndex;
  DWORD sampleIncrement;
  int endPosition;
  int sampleVolume;
  int sampleDelay;
  int amigaFrequency;
  int frequencyLookup;
  int effect;
  int effectCounter;
  int effectParam;
  int portaSpeed;
  int noteToPortaTo;
  int portaInSemitones;
  int vibratoDepth;
  int vibratoIndex;
  int vibratoNegative;
  int vibratoSpeed;
  int vibratoWaveform;
  int tremoloDepth;
  int tremoloIndex;
  int tremoloNegative;
  int tremoloSpeed;
  int tremoloWaveform;
};

static WORD readHiLoWord(FILE *);

char sb_mod_error[64];

int sb_mod_active;
int sb_mod_volume;

static struct channelInfo ci[MAX_CHANNELS];
static int totalPatterns;
static DWORD magic;
static unsigned currentPattern;
static int currentRow;
static int tickHz;
static int speed;
static int currentTick;
static int currentOrderIndex;
static BYTE *currentNote;
static sb_mod_file *currentMod;
static int bytesLeft;
static int needToJump;
static int rowToJumpTo;
static int indexToJumpTo;
static int patternLoopRow, patternLoopTimes;
static int patternDelay;

#ifdef _SB_MOD_SHOW_INFO
static unsigned mainCount;
#endif

static int frequency[296]={ 907, 900, 894, 887, 881, 875, 868, 862, 856,
                                 850, 844, 838, 832, 826, 820, 814, 808,
                                 802, 796, 791, 785, 779, 774, 768, 762,
                                 757, 752, 746, 741, 736, 730, 725, 720,
                                 715, 709, 704, 699, 694, 689, 684, 678,
                                 675, 670, 665, 660, 655, 651, 646, 640,
                                 636, 632, 628, 623, 619, 614, 610, 604,
                                 601, 597, 592, 588, 584, 580, 575, 570,
                                 567, 563, 559, 555, 551, 547, 543, 538,
                                 535, 532, 528, 524, 520, 516, 513, 508,
                                 505, 502, 498, 494, 491, 487, 484, 480,
                                 477, 474, 470, 467, 463, 460, 457, 453,
                                 450, 447, 444, 441, 437, 434, 431, 428,
                                 425, 422, 419, 416, 413, 410, 407, 404,
                                 401, 398, 395, 392, 390, 387, 384, 381,
                                 379, 376, 373, 370, 368, 365, 363, 360,
                                 357, 355, 352, 350, 347, 345, 342, 339,
                                 337, 335, 332, 330, 328, 325, 323, 320,
                                 318, 316, 314, 312, 309, 307, 305, 302,
                                 300, 298, 296, 294, 292, 290, 288, 285,
                                 284, 282, 280, 278, 276, 274, 272, 269,
                                 268, 266, 264, 262, 260, 258, 256, 254,
                                 253, 251, 249, 247, 245, 244, 242, 240,
                                 238, 237, 235, 233, 232, 230, 228, 226,
                                 225, 223, 222, 220, 219, 217, 216, 214,
                                 212, 211, 209, 208, 206, 205, 203, 202,
                                 200, 199, 198, 196, 195, 193, 192, 190,
                                 189, 188, 187, 185, 184, 183, 181, 180,
                                 179, 177, 176, 175, 174, 172, 171, 170,
                                 169, 167, 166, 165, 164, 163, 161, 160,
                                 159, 158, 157, 156, 155, 154, 152, 151,
                                 150, 149, 148, 147, 146, 145, 144, 143,
                                 142, 141, 140, 139, 138, 137, 136, 135,
                                 134, 133, 132, 131, 130, 129, 128, 127,
                                 126, 125, 125, 123, 123, 122, 121, 120,
                                 119, 118, 118, 117, 116, 115, 114, 113,
                                 113, 112, 111, 110, 109, 109, 108 };

static int sineTable[32]={   0,  24,  49,  74,  97, 120, 141, 161,
                           180, 197, 212, 224, 235, 244, 250, 253,
                           255, 253, 250, 244, 235, 224, 212, 197,
                           180, 161, 141, 120,  97,  74,  49,  24 };


static int topOfFunctions(void) { }

#ifdef _SB_MOD_SHOW_INFO

static void timerOn(void) {
  outportb(0x43,0xB0);
  outportb(0x42,0x00);
  outportb(0x42,0x00);
}

static int timerOff(void) {
  int elapsed;

  outportb(0x43,0xC0);
  elapsed=inportb(0x42)+inportb(0x42)*256;
  elapsed=0x10000-elapsed;
  return elapsed;
}

static void pprintf(char *string, int x, int y) {
  unsigned sptr=0xB8000+(((y<<6)+(y<<4)+x)*2);

  _farsetsel(_go32_info_block.selector_for_linear_memory);
  while(*string) {
    _farnspokeb(sptr,*string);
    ++string;
    sptr+=2;
  };
}
#endif

static void parseRow(void) {
  int a;
  unsigned sample, temp;
  int efCom, lookup;

  if(needToJump) {
    needToJump=0;
    currentOrderIndex=indexToJumpTo;
    currentPattern=currentMod->arrangement[currentOrderIndex];
    currentNote=currentMod->patternBuffer+(currentPattern*currentMod->modChannels*4*64);
    if(rowToJumpTo)
      currentNote+=(rowToJumpTo*currentMod->modChannels*4);
    currentRow=rowToJumpTo;
  }

  for(a=0;a<currentMod->modChannels;a++) {
    sample=currentNote[0];
    efCom=currentNote[1]>>4;
    lookup=(((DWORD)(currentNote[1]&0x0F))<<8)|currentNote[2];
    ci[a].effectParam=currentNote[3];

    if(sample&&(efCom!=3)&&(efCom!=5)) {
      ci[a].effect=NONE;
      sample-=1;
      ci[a].sample=sample;
      ci[a].sampleData=currentMod->instrument[sample].sample;
      ci[a].samplePosition=0;
      ci[a].endPosition=currentMod->instrument[sample].length;
      ci[a].sampleVolume=currentMod->instrument[sample].loudness;
    }
    if((lookup!=0x0FFF)&&(efCom!=3)&&(efCom!=5)) {
      ci[a].effect=NONE;
      ci[a].frequencyLookup=lookup;
      ci[a].amigaFrequency=frequency[lookup+(int)currentMod->instrument[ci[a].sample].fineTune];
      ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
      ci[a].vibratoIndex=0;
      ci[a].vibratoNegative=0;
      ci[a].tremoloIndex=0;
      ci[a].tremoloNegative=0;
    }

    switch(efCom) {
      case 0x00:
        if((ci[a].effect==NONE)&&(ci[a].effectParam!=0)) {
          ci[a].effect=ARPEGGIO;
          ci[a].effectCounter=1;
        }
        break;
      case 0x01:
        ci[a].effect=PORTA_UP;
        break;
      case 0x02:
        ci[a].effect=PORTA_DOWN;
        break;
      case 0x03:
        ci[a].effect=PORTA_TO_NOTE;
        if(ci[a].effectParam)
          ci[a].portaSpeed=ci[a].effectParam;
        if(lookup!=0x0FFF)
          ci[a].noteToPortaTo=frequency[lookup+(int)currentMod->instrument[ci[a].sample].fineTune];
        break;
      case 0x04:
        ci[a].effect=VIBRATO;
        if(ci[a].effectParam) {
          ci[a].vibratoSpeed=(ci[a].effectParam>>4);
          ci[a].vibratoDepth=(ci[a].effectParam&0x0F);
        }
        break;
      case 0x05:
        ci[a].effect=PORTA_VOLUME_SLIDE;
        break;
      case 0x06:
        ci[a].effect=VIBRATO_VOLUME_SLIDE;
        break;
      case 0x07:
        ci[a].effect=TREMOLO;
        if(ci[a].effectParam) {
          ci[a].tremoloSpeed=(ci[a].effectParam>>4);
          ci[a].tremoloDepth=(ci[a].effectParam&0x0F);
        }
        break;
      case 0x09:
        ci[a].samplePosition=ci[a].effectParam;
        ci[a].samplePosition<<=8;
        if(ci[a].samplePosition>=ci[a].endPosition)
          ci[a].samplePosition=0;
        else
          ci[a].samplePosition<<=16;
        break;
      case 0x0A:
        ci[a].effect=VOLUME_SLIDE;
        break;
      case 0x0B:
        needToJump=1;
        rowToJumpTo=0;
        if(ci[a].effectParam<currentMod->songLength)
          indexToJumpTo=ci[a].effectParam;
        else
          indexToJumpTo=currentMod->songLength-1;
        break;
      case 0x0C:
        ci[a].sampleVolume=ci[a].effectParam <= 0x40 ? ci[a].effectParam : 0x40;
        break;
      case 0x0D:
        needToJump=1;
        indexToJumpTo=currentOrderIndex+1;
        temp=((ci[a].effectParam>>4)*10)+(ci[a].effectParam&0x0F);
        rowToJumpTo = temp < 64 ? temp : 0;
        if(indexToJumpTo>=currentMod->songLength)
          sb_mod_active=0;
        break;
      case 0x0E:
        switch(ci[a].effectParam>>4) {
          case 0x01:
            ci[a].amigaFrequency-=(ci[a].effectParam&0x0F);
            ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
            break;
          case 0x02:
            ci[a].amigaFrequency+=(ci[a].effectParam&0x0F);
            ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
            break;
          case 0x03:
            ci[a].portaInSemitones=(ci[a].effectParam&0x0F);
            break;
          case 0x04:
            ci[a].vibratoWaveform=(ci[a].effectParam&0x0F);
            break;
          case 0x05:
            temp=(ci[a].effectParam&0x0F);
            if(temp>7)
              temp-=16;
            currentMod->instrument[sample].fineTune=(ci[a].effectParam&0x0F);
            break;
          case 0x06:
            temp=(ci[a].effectParam&0x0F);
            if(temp==0)
              patternLoopRow=currentRow;
            else {
              if(patternLoopTimes==0)
                patternLoopTimes=temp;
              else
                patternLoopTimes-=1;
              if(patternLoopTimes>0) {
                needToJump=1;
                rowToJumpTo=patternLoopRow;
                indexToJumpTo=currentOrderIndex;
              }
            }
            break;
          case 0x07:
            ci[a].tremoloWaveform=(ci[a].effectParam&0x0F);
            break;
          case 0x09:
            ci[a].effect=RETRIG_NOTE;
            ci[a].effectCounter=(ci[a].effectParam&0x0F);
            break;
          case 0x0A:
            ci[a].sampleVolume+=(ci[a].effectParam&0x0F);
            if(ci[a].sampleVolume>0x40)
              ci[a].sampleVolume=0x40;
            break;
          case 0x0B:
            ci[a].sampleVolume-=(ci[a].effectParam&0x0F);
            if(ci[a].sampleVolume<0)
              ci[a].sampleVolume=0;
            break;
          case 0x0C:
            ci[a].effect=CUT_NOTE;
            ci[a].effectCounter=(ci[a].effectParam&0x0F);
            break;
          case 0x0D:
            ci[a].sampleDelay=(ci[a].effectParam&0x0F);
            break;
          case 0x0E:
            patternDelay=(ci[a].effectParam&0x0F);
            break;
        }
        break;
      case 0x0F:
        if(ci[a].effectParam<0x20)
          speed=ci[a].effectParam;
        else
          tickHz=(ci[a].effectParam<<1)/5;
        break;
    }
    currentNote+=4;
  }
  ++currentRow;
}

void updateEffects(void) {
  int a;
  unsigned x, y;

  for(a=0;a<currentMod->modChannels;a++) {

    if(ci[a].sampleDelay>0)
      ci[a].sampleDelay-=1;

    x=ci[a].effectParam>>4;
    y=ci[a].effectParam&0x0F;

    switch(ci[a].effect) {
      case ARPEGGIO:
        if(ci[a].effectCounter==1)
          ci[a].amigaFrequency=frequency[ci[a].frequencyLookup+(x*8)+currentMod->instrument[ci[a].sample].fineTune];
        else if(ci[a].effectCounter==2)
          ci[a].amigaFrequency=frequency[ci[a].frequencyLookup+(y*8)+currentMod->instrument[ci[a].sample].fineTune];
        else
          ci[a].amigaFrequency=frequency[ci[a].frequencyLookup+currentMod->instrument[ci[a].sample].fineTune];
        if(ci[a].amigaFrequency!=0)
          ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
        ci[a].effectCounter+=1;
        if(ci[a].effectCounter>2)
          ci[a].effectCounter=0;
        break;
      case PORTA_DOWN:
        ci[a].amigaFrequency+=ci[a].effectParam;
        if(ci[a].amigaFrequency>1800)
          ci[a].amigaFrequency=1800;
        ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
        break;
      case PORTA_UP:
        ci[a].amigaFrequency-=ci[a].effectParam;
        if(ci[a].amigaFrequency<54)
          ci[a].amigaFrequency=54;
        ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
        break;
      case PORTA_TO_NOTE:
      case PORTA_VOLUME_SLIDE:
        if(ci[a].amigaFrequency<ci[a].noteToPortaTo) {
          ci[a].amigaFrequency+=ci[a].portaSpeed;
          if(ci[a].amigaFrequency>ci[a].noteToPortaTo)
            ci[a].amigaFrequency=ci[a].noteToPortaTo;
        }
        else if(ci[a].amigaFrequency>ci[a].noteToPortaTo) {
          ci[a].amigaFrequency-=ci[a].portaSpeed;
          if(ci[a].amigaFrequency<ci[a].noteToPortaTo)
            ci[a].amigaFrequency=ci[a].noteToPortaTo;
        }
        ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
        if(ci[a].effect==PORTA_VOLUME_SLIDE)
          goto doVolume;
        break;
      case CUT_NOTE:
        if(ci[a].effectCounter>0)
          ci[a].effectCounter-=1;
        else
          ci[a].sampleVolume=0;
        break;
      case VOLUME_SLIDE:
       doVolume:
        if((x>0)&&(y==0)) {
          ci[a].sampleVolume+=x;
          if(ci[a].sampleVolume>0x40)
            ci[a].sampleVolume=0x40;
        }
        else if((y>0)&&(x==0)) {
          ci[a].sampleVolume-=y;
          if(ci[a].sampleVolume<0)
            ci[a].sampleVolume=0;
        }
        break;
      case VIBRATO:
      case VIBRATO_VOLUME_SLIDE:
        y=(ci[a].vibratoDepth*sineTable[ci[a].vibratoIndex])>>7;
        if(ci[a].vibratoNegative)
          ci[a].amigaFrequency+=y;
        else
          ci[a].amigaFrequency-=y;
        ci[a].sampleIncrement=magic/ci[a].amigaFrequency;
        ci[a].vibratoIndex+=ci[a].vibratoSpeed;
        if(ci[a].vibratoIndex>31) {
          ci[a].vibratoIndex-=32;
          ci[a].vibratoNegative^=1;
        }
        if(ci[a].effect==VIBRATO_VOLUME_SLIDE)
          goto doVolume;
        break;
      case TREMOLO:
        y=(ci[a].tremoloDepth*sineTable[ci[a].tremoloIndex])>>6;
        if(ci[a].tremoloNegative) {
          ci[a].sampleVolume-=y;
          if(ci[a].sampleVolume<0)
            ci[a].sampleVolume=0;
        }
        else {
          ci[a].sampleVolume+=y;
          if(ci[a].sampleVolume>0x40)
            ci[a].sampleVolume=0x40;
        }
        ci[a].tremoloIndex+=ci[a].tremoloSpeed;
        if(ci[a].tremoloIndex>31) {
          ci[a].tremoloIndex-=32;
          ci[a].tremoloNegative^=1;
        }
        break;
      case RETRIG_NOTE:
        if(ci[a].effectCounter)
          if(currentTick%ci[a].effectCounter==0)
            ci[a].samplePosition=0;
        break;
    }
  }

}

/* Optimize this function to speed up the .MOD player. */
inline static void doMix(int ch, int *buf, int size) {
  int a;

  if(ci[ch].sampleVolume&&(*ci[ch].sampleIndex<=ci[ch].endPosition)&&(ci[ch].sampleDelay==0)) {
    for(a=0;a<size;a++) {
      *(buf+a)+=(sb_multTable[ci[ch].sampleVolume-1][(unsigned char)*(ci[ch].sampleData + *ci[ch].sampleIndex)])>>sb_mod_volume;
      ci[ch].samplePosition+=ci[ch].sampleIncrement;
      if(*(ci[ch].sampleIndex)>=ci[ch].endPosition) {
        if(currentMod->instrument[ci[ch].sample].repeatLength>1) {
          ci[ch].samplePosition=currentMod->instrument[ci[ch].sample].repeatStart;
          ci[ch].samplePosition<<=16;
          ci[ch].endPosition=currentMod->instrument[ci[ch].sample].repeatStart+currentMod->instrument[ci[ch].sample].repeatLength;
        }
        else
          break;
      }
    }
  }
}

void sb_mod_parse_file(void) {
  int a;
  int thisByte;

#ifdef _SB_MOD_SHOW_INFO
  timerOn();
#endif

  if(bytesLeft>(_SB_BLOCK_SIZE/2)) {
    if(currentMod->modChannels==4) {
      doMix(0,sb_left_buffer,_SB_BLOCK_SIZE/2);
      doMix(3,sb_left_buffer,_SB_BLOCK_SIZE/2);
      doMix(1,sb_right_buffer,_SB_BLOCK_SIZE/2);
      doMix(2,sb_right_buffer,_SB_BLOCK_SIZE/2);
    }
    else {
      for(a=0;a<(currentMod->modChannels/2);a++)
        doMix(a,sb_left_buffer,_SB_BLOCK_SIZE/2);
      for(a=(currentMod->modChannels/2);a<currentMod->modChannels;a++)
        doMix(a,sb_right_buffer,_SB_BLOCK_SIZE/2);
    }
    bytesLeft-=(_SB_BLOCK_SIZE/2);
  }
  else {
    if(currentMod->modChannels==4) {
      doMix(0,sb_left_buffer,bytesLeft);
      doMix(3,sb_left_buffer,bytesLeft);
      doMix(1,sb_right_buffer,bytesLeft);
      doMix(2,sb_right_buffer,bytesLeft);
    }
    else {
      for(a=0;a<(currentMod->modChannels/2);a++)
        doMix(a,sb_left_buffer,bytesLeft);
      for(a=(currentMod->modChannels/2);a<currentMod->modChannels;a++)
        doMix(a,sb_right_buffer,bytesLeft);
    }

    thisByte=bytesLeft;

    if(currentTick>=speed) {
      if(patternDelay==0) {
        parseRow();
        if(currentRow==64) {
          ++currentOrderIndex;
          if((currentOrderIndex>=currentMod->songLength)&&(!needToJump))
            sb_mod_active=0;
          else {
            currentPattern=currentMod->arrangement[currentOrderIndex];
            currentNote=currentMod->patternBuffer+(currentPattern*currentMod->modChannels*4*64);
            currentRow=0;
          }
        }
      }
      else
        patternDelay-=1;
      currentTick=0;
    }
    else {
      updateEffects();
    }

    bytesLeft=sb_sample_frequency/tickHz;
    currentTick++;

    if(currentMod->modChannels==4) {
      doMix(0,sb_left_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);
      doMix(3,sb_left_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);
      doMix(1,sb_right_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);
      doMix(2,sb_right_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);
    }
    else {
      for(a=0;a<(currentMod->modChannels/2);a++)
        doMix(a,sb_left_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);
      for(a=(currentMod->modChannels/2);a<currentMod->modChannels;a++)
        doMix(a,sb_right_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);
    }

    bytesLeft-=((_SB_BLOCK_SIZE/2)-thisByte);
  }
#ifdef _SB_MOD_SHOW_INFO
  mainCount+=timerOff();
#endif
}

void sb_mod_parse_file_mono(void) {
  int a;
  int thisByte;

#ifdef _SB_MOD_SHOW_INFO
  timerOn();
#endif

  if(bytesLeft>(_SB_BLOCK_SIZE/2)) {
    for(a=0;a<currentMod->modChannels;a++)
      doMix(a,sb_mixing_buffer,_SB_BLOCK_SIZE/2);
    bytesLeft-=(_SB_BLOCK_SIZE/2);
  }
  else {
    for(a=0;a<currentMod->modChannels;a++)
      doMix(a,sb_mixing_buffer,bytesLeft);

    thisByte=bytesLeft;

    if(currentTick>=speed) {
      if(patternDelay==0) {
        parseRow();
        if(currentRow==64) {
          ++currentOrderIndex;
          if((currentOrderIndex>=currentMod->songLength)&&(!needToJump))
            sb_mod_active=0;
          else {
            currentPattern=currentMod->arrangement[currentOrderIndex];
            currentNote=currentMod->patternBuffer+(currentPattern*currentMod->modChannels*4*64);
            currentRow=0;
          }
        }
      }
      else
        patternDelay-=1;
      currentTick=0;
    }
    else {
      updateEffects();
    }

    bytesLeft=sb_sample_frequency/tickHz;
    currentTick++;

    for(a=0;a<currentMod->modChannels;a++)
      doMix(a,sb_mixing_buffer+thisByte,(_SB_BLOCK_SIZE/2)-thisByte);

    bytesLeft-=((_SB_BLOCK_SIZE/2)-thisByte);
  }
#ifdef _SB_MOD_SHOW_INFO
  mainCount+=timerOff();
#endif
}

static int bottomOfFunctions(void) { }

void sb_mod_initialize_player(void) {
  double temp;
  int a, b;
  char c;

  tickHz=50;
  sb_mod_volume=7;
  sb_mod_active=0;


/*
           Hz = 3579545.25 / AmigaFrequency

and

           SampleIncrement = Hz / sb_sample_frequency

therefore
                              3579545.25/AmigaFrequency
           SampleIncrement =  -------------------------
                                sb_sample_frequency

and therefore,

                              3579545.25/sb_sample_frequency
           SampleIncrement =  ------------------------------
                                     AmigaFrequency

Since I don't know what AmigaFrequency will be ahead of time, the best I can do
is store the numerator (3579545.25/sb_sample_frequency).

*/

  temp=3579545.25/(double)sb_sample_frequency;

/*
  Now, since SampleIncrement will be a fixed-point value, we'll multiply it by
65536, thus moving the integral part into the high-word, and the fractional
part into the low-word.
*/

  magic=temp*65536.0;   /* The numerator in our SampleIncrement formula */
}

int sb_mod_module_lock_your_memory(void) {
  if((_go32_dpmi_lock_data(ci,MAX_CHANNELS*sizeof(struct channelInfo))) ||
     (_go32_dpmi_lock_data(&ci,sizeof(struct channelInfo *))) ||
     (_go32_dpmi_lock_data(&sb_mod_active,sizeof(int))) ||
     (_go32_dpmi_lock_data(&sb_mod_volume,sizeof(int))) ||
     (_go32_dpmi_lock_data(&magic,sizeof(DWORD))) ||
     (_go32_dpmi_lock_data(&currentPattern,sizeof(unsigned))) ||
     (_go32_dpmi_lock_data(&currentRow,sizeof(int))) ||
     (_go32_dpmi_lock_data(&tickHz,sizeof(int))) ||
     (_go32_dpmi_lock_data(&speed,sizeof(int))) ||
     (_go32_dpmi_lock_data(&currentTick,sizeof(int))) ||
     (_go32_dpmi_lock_data(&currentOrderIndex,sizeof(int))) ||
     (_go32_dpmi_lock_data(&currentNote,sizeof(BYTE *))) ||
     (_go32_dpmi_lock_data(&currentMod,sizeof(sb_mod_file *))) ||
     (_go32_dpmi_lock_data(&bytesLeft,sizeof(int))) ||
     (_go32_dpmi_lock_data(&needToJump,sizeof(int))) ||
     (_go32_dpmi_lock_data(&rowToJumpTo,sizeof(int))) ||
     (_go32_dpmi_lock_data(&indexToJumpTo,sizeof(int))) ||
     (_go32_dpmi_lock_data(&patternLoopRow,sizeof(int))) ||
     (_go32_dpmi_lock_data(&patternLoopTimes,sizeof(int))) ||
     (_go32_dpmi_lock_data(&patternDelay,sizeof(int))) ||
#ifdef _SB_MOD_SHOW_INFO
     (_go32_dpmi_lock_data(&mainCount,sizeof(unsigned))) ||
#endif
     (_go32_dpmi_lock_data(frequency,296*sizeof(int))) ||
     (_go32_dpmi_lock_data(&frequency,sizeof(int *))) ||
     (_go32_dpmi_lock_data(sineTable,32*sizeof(int))) ||
     (_go32_dpmi_lock_data(&sineTable,sizeof(int *))) ||
     (_go32_dpmi_lock_code(topOfFunctions,((char *)bottomOfFunctions-(char *)topOfFunctions))))
    return 0;
  return 1;
}

void sb_mod_play(sb_mod_file *mod) {
  int a;

  if(mod!=NULL) {
    sb_mod_active=0;            /* Don't call the driver while we do this! */
    speed=6;
    currentTick=speed;
    currentMod=mod;
    currentOrderIndex=0;
    currentPattern=currentMod->arrangement[currentOrderIndex];
    currentNote=currentMod->patternBuffer+(currentPattern*currentMod->modChannels*4*64);
    currentRow=0;
    bytesLeft=0;
    for(a=0;a<MAX_CHANNELS;a++) {
      ci[a].samplePosition=65536;
      ci[a].endPosition=0;
      ci[a].sampleIndex=(WORD *)(((BYTE *)&ci[a].samplePosition)+2);
      ci[a].sampleDelay=0;
      ci[a].portaInSemitones=0;
      ci[a].vibratoWaveform=0;
      ci[a].tremoloWaveform=0;
      ci[a].portaInSemitones=0;
      ci[a].effect=NONE;
      ci[a].effectParam=0;
      ci[a].amigaFrequency=0x0FFF;
      ci[a].frequencyLookup=0;
    }
    needToJump=0;
    patternLoopTimes=0;
    patternDelay=0;
    sb_mod_active=1;
  }
}

void sb_mod_pause(void) {
  sb_mod_active=0;
}

void sb_mod_resume(void) {
  sb_mod_active=1;
}

sb_mod_file *sb_load_mod_file(char *fname) {
  FILE *fp;
  sb_mod_file *mod;
  int a, b, c, d;
  int fileLength;
  BYTE *base;
  char buf[4];
  char buffer[80];
  unsigned sampleNum, periodFreq, effectNum, effectParam;

  mod=(sb_mod_file *)malloc(sizeof(sb_mod_file));
  if(mod==NULL) {
    strcpy(sb_mod_error,"Out of memory!");
    return mod;
  }
  if(_go32_dpmi_lock_data(mod,sizeof(sb_mod_file))) {
    strcpy(sb_mod_error,"Could not lock appropriate memory (sb_mod_file)");
    free(mod);
    return NULL;
  }

  fp=fopen(fname,"rb");
  if(fp==NULL) {
    strcpy(sb_mod_error,"File not found!");
    free(mod);
    return NULL;
  }

  fseek(fp,0,SEEK_END);
  fileLength=ftell(fp);
  fseek(fp,1080,SEEK_SET);
  fread(mod->id,1,4,fp);
  fseek(fp,0,SEEK_SET);

  if((memcmp("M.K.",mod->id,4)==0)||(memcmp("FLT4",mod->id,4)==0)||(memcmp("M!K!",mod->id,4)==0)) {
    mod->modSamples=31;
    mod->modChannels=4;
  }
  else if(memcmp("6CHN",mod->id,4)==0) {
    mod->modSamples=31;
    mod->modChannels=6;
  }
  else if(memcmp("8CHN",mod->id,4)==0) {
    mod->modSamples=31;
    mod->modChannels=8;
  }
  else {                         /* Otherwise assume old-style .MOD (N.T.) */
    mod->modSamples=15;
    mod->modChannels=4;
  }

  mod->instrument=(sb_mod_instrument *)malloc(mod->modSamples*sizeof(sb_mod_instrument));
  if(mod->instrument==NULL) {
    strcpy(sb_mod_error,"Out of memory!");
    fclose(fp);
    free(mod);
    return NULL;
  }
  if(_go32_dpmi_lock_data(mod->instrument,mod->modSamples*sizeof(sb_mod_instrument))) {
    strcpy(sb_mod_error,"Could not lock appropriate memory (mod->instrument)");
    fclose(fp);
    free(mod->instrument);
    free(mod);
    return NULL;
  }

  fread(mod->title,1,20,fp);

  for(a=0;a<mod->modSamples;a++) {
    fread(mod->instrument[a].name,1,22,fp);
    mod->instrument[a].name[21]=0;
    mod->instrument[a].length=readHiLoWord(fp);
    mod->instrument[a].fineTune=fgetc(fp);
    if(mod->instrument[a].fineTune>7)
      mod->instrument[a].fineTune-=16;
    mod->instrument[a].loudness=fgetc(fp);
    mod->instrument[a].repeatStart=readHiLoWord(fp);
    mod->instrument[a].repeatLength=readHiLoWord(fp);
    if(mod->instrument[a].repeatLength==2)
      mod->instrument[a].repeatLength=0;
    if(mod->instrument[a].repeatStart+mod->instrument[a].repeatLength > mod->instrument[a].length) {
      if(mod->instrument[a].repeatLength > mod->instrument[a].length) {
        mod->instrument[a].repeatLength=mod->instrument[a].length;
        mod->instrument[a].repeatStart=0;
      }
      else
        mod->instrument[a].repeatStart-=(mod->instrument[a].repeatLength+mod->instrument[a].repeatStart-mod->instrument[a].length);
    }
  }

  mod->songLength=fgetc(fp);
  if(mod->songLength>128) {
    fclose(fp);
    free(mod->instrument);
    free(mod);
    strcpy(sb_mod_error,"Bad .MOD file!");
    return NULL;
  }
  mod->CIAA=fgetc(fp);
  fread(mod->arrangement,1,128,fp);

  if(mod->modSamples==31) {
    fread(buf,1,4,fp);
    if(memcmp(buf,mod->id,4)!=0) {      /* Double check ID */
      fclose(fp);
      free(mod->instrument);
      free(mod);
      strcpy(sb_mod_error,"Bad .MOD file!");
      return NULL;
    }
  }

  b=0;
  for(a=0;a<128;a++) {
    if(mod->arrangement[a]>b)
      b=mod->arrangement[a];
  }
  b++;
  totalPatterns=b;

  mod->patternBuffer=(BYTE *)malloc(b*mod->modChannels*4*64);
  if(mod->patternBuffer==NULL) {
    strcpy(sb_mod_error,"Out of memory!");
    fclose(fp);
    free(mod->instrument);
    free(mod);
    return NULL;
  }
  if(_go32_dpmi_lock_data(mod->patternBuffer,b*mod->modChannels*4*64)) {
    strcpy(sb_mod_error,"Could not lock appropriate memory (mod->patternBuffer)");
    fclose(fp);
    free(mod->instrument);
    free(mod->patternBuffer);
    free(mod);
    return NULL;
  }
  fread(mod->patternBuffer,1,b*mod->modChannels*4*64,fp);

  for(a=0;a<b;a++) {
    base=mod->patternBuffer+(a*mod->modChannels*4*64);
    for(c=0;c<64*mod->modChannels;c++,base+=4) {
      sampleNum=(base[0]&0xF0)|(base[2]>>4);
      periodFreq=(((DWORD)base[0]&0x0F)<<8)|base[1];
      effectNum=base[2]&0x0F;
      effectParam=base[3];
      for(d=1;d<=36;d++)
        if((periodFreq>frequency[d*8]-2)&&(periodFreq<frequency[d*8]+2)) {
          periodFreq=d*8;
          d=37;
        }
      if(d!=38)
        periodFreq=0x0FFF;              /* This means DON'T PLAY ME! */

      base[0]=sampleNum;
      base[1]=(effectNum<<4)|(periodFreq>>8);
      base[2]=periodFreq&0xFF;
      base[3]=effectParam;
    }
  }

  for(a=0;a<mod->modSamples;a++) {
    mod->instrument[a].sample=(char *)malloc(mod->instrument[a].length);
    if(mod->instrument[a].sample==NULL) {
      strcpy(sb_mod_error,"Out of memory!");
      fclose(fp);
      for(d=a-1;d>=0;d--)
        free(mod->instrument[d].sample);
      free(mod->instrument);
      free(mod->patternBuffer);
      free(mod);
      return NULL;
    }
    if(_go32_dpmi_lock_data(mod->instrument[a].sample,mod->instrument[a].length)) {
      strcpy(sb_mod_error,"Could not lock appropriate memory (mod->instrument.sample)");
      fclose(fp);
      for(d=a;d>=0;d--)
        free(mod->instrument[d].sample);
      free(mod->instrument);
      free(mod->patternBuffer);
      free(mod);
      return NULL;
    } 
    fread(mod->instrument[a].sample,1,mod->instrument[a].length,fp);
  }

  fclose(fp);
  return mod;
}

void sb_free_mod_file(sb_mod_file *mod) {
  int a;

  for(a=0;a<mod->modSamples;a++) {
    free(mod->instrument[a].sample);
  }
  free(mod->instrument);
  free(mod->patternBuffer);
  free(mod);
}

static WORD readHiLoWord(FILE *file) {
  unsigned lo, hi;

  hi=fgetc(file);
  lo=fgetc(file);
  hi<<=8;
  hi|=lo;
  hi<<=1;
  return (WORD)hi;
}

#ifdef _SB_MOD_SHOW_INFO
void sb_mod_show_info(void) {
  int a, b;
  char buffer[80];

  currentMod->title[19]=0;
  sprintf(buffer,"Name: %s",currentMod->title);
  pprintf(buffer,0,0);

  sprintf(buffer,"Channels: %d",currentMod->modChannels);
  pprintf(buffer,29,0);

  sprintf(buffer,"ID: %c%c%c%c",currentMod->id[0],currentMod->id[1],
                         currentMod->id[2],currentMod->id[3]);
  pprintf(buffer,43,0);

  sprintf(buffer,"Length: %d",currentMod->songLength);
  pprintf(buffer,53,0);

  pprintf("Ch Note Sample                Index Vol Effect                    Param",0,2);
  pprintf("-- ---- --------------------- ----- --- ------------------------- -----",0,3);

  for(a=0;a<currentMod->modChannels;a++) {
    sprintf(buffer,"%d",a);
    pprintf(buffer,0,a+4);
  }

  pprintf("Approximate CPU overhead: ",0,15);

  do {
    for(b=0;b<200;b++) {
      for(a=0;a<currentMod->modChannels;a++) {
        sprintf(buffer,"%-4d",ci[a].amigaFrequency);
        pprintf(buffer,3,a+4);

        sprintf(buffer,"%-21s",currentMod->instrument[ci[a].sample].name);
        pprintf(buffer,8,a+4);

        sprintf(buffer,"%-5d",*ci[a].sampleIndex);
        pprintf(buffer,30,a+4);

        sprintf(buffer,"%-3d",ci[a].sampleVolume);
        pprintf(buffer,36,a+4);

        sprintf(buffer,"                         ");
        switch(ci[a].effect) {
          case ARPEGGIO:
            if(ci[a].effectParam)
              sprintf(buffer,"Arpeggio                 ");
            break;
          case PORTA_UP:
            sprintf(buffer,"Portamento Up            ");
            break;
          case PORTA_DOWN:
            sprintf(buffer,"Portamento Down          ");
            break;
          case PORTA_TO_NOTE:
            sprintf(buffer,"Portamento to Note       ");
            break;
          case VIBRATO:
            sprintf(buffer,"Vibrato                  ");
            break;
          case PORTA_VOLUME_SLIDE:
            sprintf(buffer,"Portamento + Volume Slide");
            break;
          case VIBRATO_VOLUME_SLIDE:
            sprintf(buffer,"Vibrato + Volume Slide   ");
            break;
          case TREMOLO:
            sprintf(buffer,"Tremolo                  ");
            break;
          case VOLUME_SLIDE:
            sprintf(buffer,"Volume Slide             ");
            break;
          case RETRIG_NOTE:
            sprintf(buffer,"Set retrig for note      ");
            break;
        }
        pprintf(buffer,40,a+4);

        sprintf(buffer,"%-2x",ci[a].effectParam);
        pprintf(buffer,66,a+4);
      }
      sprintf(buffer,"Index: %d       Pattern: %d        Row: %d  ",currentOrderIndex,currentPattern,currentRow);
      pprintf(buffer,0,13);
    }

    sprintf(buffer,"%.2f%%  ",(mainCount*0.9218)/(200*_SB_BLOCK_SIZE)*sb_sample_frequency/11000.0);
    pprintf(buffer,26,15);
    mainCount=0;

  } while(!kbhit());
}
#endif
