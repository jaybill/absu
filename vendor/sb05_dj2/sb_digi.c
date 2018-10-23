#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sb_defs.h"
#include "sb_misc.h"


typedef struct {                /* Just a little structure to keep track of */
  sb_sample *sample;            /* how much of a sample I have played.      */
  int elapsedBytes;
  int bytesLeft;
} sampleTracker;

#ifdef _SB_INCLUDE_FILE_UTILS
static int loadRaw(FILE *, sb_sample *);
static int loadWav(FILE *, sb_sample *);
static int loadVoc(FILE *, sb_sample *);
#endif

char sb_digi_error[64];
int *sb_mixing_buffer;
int *sb_left_buffer, *sb_right_buffer;

int sb_multTable[64][256];

sb_sample *sb_queue[_SB_MAX_QUEUE];
int sb_numInQueue;
sampleTracker sb_mix[_SB_MAX_MIX];
int sb_numInMix;

static int topOfFunctions(void) { }

void sb_mix_sample(sb_sample *ss) {
  int a;

  if(ss!=NULL) {                /* Make sure it's a valid pointer. */
    if(sb_numInMix>=_SB_MAX_MIX) {
      sb_numInMix-=1;
      for(a=0;a<sb_numInMix;a++)
        sb_mix[a]=sb_mix[a+1];
    }
    sb_mix[sb_numInMix].elapsedBytes=0;
    sb_mix[sb_numInMix].bytesLeft=ss->length;
    sb_mix[sb_numInMix].sample=ss;
    sb_numInMix++;
  }
}

void sb_mix_mono_samples(void) {
  int a, b;
  int temp;

  for(a=0;a<sb_numInMix;a++) {
    temp=(sb_mix[a].sample->left_volume+sb_mix[a].sample->right_volume)>>1;
    if(sb_mix[a].bytesLeft>(_SB_BLOCK_SIZE/2)) {
      for(b=0;b<(_SB_BLOCK_SIZE/2);b++)
        sb_mixing_buffer[b]+=(sb_multTable[temp][sb_mix[a].sample->data[sb_mix[a].elapsedBytes++]])>>5;
      sb_mix[a].bytesLeft-=(_SB_BLOCK_SIZE/2);
    }
    else {
      for(b=0;b<sb_mix[a].bytesLeft;b++)
        sb_mixing_buffer[b]+=(sb_multTable[temp][sb_mix[a].sample->data[sb_mix[a].elapsedBytes++]])>>5;
      if(sb_mix[a].sample->callback!=NULL)
        sb_mix[a].sample->callback();
      if(sb_mix[a].sample==sb_queue[0]) {
        if(sb_numInQueue>1) {
          sb_mix_sample(sb_queue[1]);
          sb_numInQueue-=1;
          for(b=0;b<sb_numInQueue;b++)
            sb_queue[b]=sb_queue[b+1];
          temp=(sb_mix[sb_numInMix-1].sample->left_volume+sb_mix[sb_numInMix-1].sample->right_volume)>>1;
          for(b=sb_mix[a].bytesLeft;b<(_SB_BLOCK_SIZE/2);b++)
            sb_mixing_buffer[b]+=(sb_multTable[temp][sb_mix[sb_numInMix-1].sample->data[sb_mix[sb_numInMix-1].elapsedBytes++]])>>5;
          sb_mix[sb_numInMix-1].bytesLeft-=sb_mix[sb_numInMix-1].elapsedBytes;
        }
        else if(sb_numInQueue>0)
          sb_numInQueue-=1;
      }
      sb_numInMix-=1;
      for(b=a;b<sb_numInMix;b++)
        sb_mix[b]=sb_mix[b+1];
      a-=1;
    }
  }
}

void sb_mix_stereo_samples(void) {
  int a, b;

  for(a=0;a<sb_numInMix;a++) {
    if(sb_mix[a].bytesLeft>(_SB_BLOCK_SIZE/2)) {
      for(b=0;b<(_SB_BLOCK_SIZE/2);b++) {
        if(sb_mix[a].sample->left_volume)
          sb_left_buffer[b]+=(sb_multTable[sb_mix[a].sample->left_volume-1][sb_mix[a].sample->data[sb_mix[a].elapsedBytes]])>>5;
        if(sb_mix[a].sample->right_volume)
          sb_right_buffer[b]+=(sb_multTable[sb_mix[a].sample->right_volume-1][sb_mix[a].sample->data[sb_mix[a].elapsedBytes]])>>5;
        sb_mix[a].elapsedBytes+=1;
      }
      sb_mix[a].bytesLeft-=(_SB_BLOCK_SIZE/2);
    }
    else {
      for(b=0;b<sb_mix[a].bytesLeft;b++) {
        if(sb_mix[a].sample->left_volume)
          sb_left_buffer[b]+=(sb_multTable[sb_mix[a].sample->left_volume-1][sb_mix[a].sample->data[sb_mix[a].elapsedBytes]])>>5;
        if(sb_mix[a].sample->right_volume)
          sb_right_buffer[b]+=(sb_multTable[sb_mix[a].sample->right_volume-1][sb_mix[a].sample->data[sb_mix[a].elapsedBytes]])>>5;
        sb_mix[a].elapsedBytes+=1;
      }
      if(sb_mix[a].sample->callback!=NULL)
        sb_mix[a].sample->callback();
      if(sb_mix[a].sample==sb_queue[0]) {
        if(sb_numInQueue>1) {
          sb_mix_sample(sb_queue[1]);
          sb_numInQueue-=1;
          for(b=0;b<sb_numInQueue;b++)
            sb_queue[b]=sb_queue[b+1];
          for(b=sb_mix[a].bytesLeft;b<(_SB_BLOCK_SIZE/2);b++) {
            if(sb_mix[sb_numInMix-1].sample->left_volume)
              sb_left_buffer[b]+=(sb_multTable[sb_mix[sb_numInMix-1].sample->left_volume-1][sb_mix[sb_numInMix-1].sample->data[sb_mix[sb_numInMix-1].elapsedBytes]])>>5;
            if(sb_mix[sb_numInMix-1].sample->right_volume)
              sb_right_buffer[b]+=(sb_multTable[sb_mix[sb_numInMix-1].sample->right_volume-1][sb_mix[sb_numInMix-1].sample->data[sb_mix[sb_numInMix-1].elapsedBytes]])>>5;
            sb_mix[sb_numInMix-1].elapsedBytes+=1;
          }
          sb_mix[sb_numInMix-1].bytesLeft-=sb_mix[sb_numInMix-1].elapsedBytes;
        }
        else if(sb_numInQueue>0)
          sb_numInQueue-=1;
      }
      sb_numInMix-=1;
      for(b=a;b<sb_numInMix;b++)
        sb_mix[b]=sb_mix[b+1];
      a-=1;
    }
  }
}

static int bottomOfFunctions(void) { }

sb_status sb_queue_sample(sb_sample *ss) {
  sb_status stat=SB_SUCCESS;

  if(ss!=NULL) {
    if(sb_numInQueue>=_SB_MAX_QUEUE) {
      stat=SB_BUSY;
      return stat;
    }
    if(sb_numInQueue==0)          /* If the queue is empty right now, just */
      sb_mix_sample(ss);          /* start playing this one right away.    */
    sb_queue[sb_numInQueue]=ss;
    sb_numInQueue++;
  }
  else
    stat=SB_BAD_POINTER;
  return stat;
}

#ifdef _SB_INCLUDE_CONVERSION_UTILS
sb_sample *sb_convert_frequency(sb_sample *srcSample, int srcFreq, int destFreq) {
  sb_sample *ss;
  double ratio;
  int a;
  long long int position, end, increment;       /* 64-bit fixed-point vars */
  long *integral;
  
  ss=(sb_sample *)malloc(sizeof(sb_sample));
  if(_go32_dpmi_lock_data(ss,sizeof(sb_sample))) {
    strcpy(sb_digi_error,"Could not lock appropriate memory.");
    free(ss);
    return NULL;
  }

  ratio=(double)srcFreq/(double)destFreq;
  ss->length=(double)srcSample->length/ratio;

  end=(long long)srcSample->length<<32;
  increment=ratio*((long long)1<<32);
  integral=(long *)((BYTE *)&position+4);

  ss->data=(BYTE *)malloc(ss->length);
  if(ss->data==NULL) {
    strcpy(sb_digi_error,"Out of memory!");
    free(ss);
    return NULL;
  }
  if(_go32_dpmi_lock_data(ss->data,ss->length)) {
    strcpy(sb_digi_error,"Could not lock appropriate memory.");
    free(ss->data);
    free(ss);
    return NULL;
  }


  for(position=0,a=0;position<end;position+=increment,a++)
    ss->data[a]=srcSample->data[*integral];

  ss->stereo=srcSample->stereo;
  ss->bits=srcSample->bits;
  ss->left_volume=srcSample->left_volume;
  ss->right_volume=srcSample->right_volume;
  ss->callback=srcSample->callback;

  return ss;
}
#endif

sb_status sb_digi_initialize(void) {
  sb_status stat;
  int a, b;
  char c;

  sb_mixing_buffer=(int *)malloc((_SB_BLOCK_SIZE*2)*sizeof(int));
  if(sb_mixing_buffer==NULL) {
    strcpy(sb_digi_error,"Out of memory!");
    stat=SB_FAILURE;
    return stat;
  }

  sb_left_buffer=sb_mixing_buffer+_SB_BLOCK_SIZE;
  sb_right_buffer=sb_left_buffer+(_SB_BLOCK_SIZE/2);

  if(_go32_dpmi_lock_data(sb_mixing_buffer,(_SB_BLOCK_SIZE*2)*sizeof(int))) {
    strcpy(sb_digi_error,"Could not lock appropriate memory.");
    return SB_FAILURE;
  }

  c=0;
  for(a=0;a<256;a++) {
    for(b=1;b<=64;b++)
      sb_multTable[b-1][a]=c*b;
    ++c;
  }

  sb_numInQueue=0;
  sb_numInMix=0;

  stat=SB_SUCCESS;
  return stat;
}

int sb_digi_module_lock_your_memory(void) {
  if((_go32_dpmi_lock_data(&sb_mixing_buffer,sizeof(int *))) ||
     (_go32_dpmi_lock_data(&sb_left_buffer,sizeof(int *))) ||
     (_go32_dpmi_lock_data(&sb_right_buffer,sizeof(int *))) ||
     (_go32_dpmi_lock_data(sb_queue,_SB_MAX_QUEUE*sizeof(sb_sample *))) ||
     (_go32_dpmi_lock_data(&sb_queue,sizeof(sb_sample **))) ||
     (_go32_dpmi_lock_data(sb_mix,_SB_MAX_MIX*sizeof(sampleTracker))) ||
     (_go32_dpmi_lock_data(&sb_mix,sizeof(sampleTracker *))) ||
     (_go32_dpmi_lock_data(&sb_numInQueue,sizeof(int))) ||
     (_go32_dpmi_lock_data(&sb_numInMix,sizeof(int))) ||
     (_go32_dpmi_lock_data(sb_multTable,256*64*sizeof(int))) ||
     (_go32_dpmi_lock_data(&sb_multTable,sizeof(int *))) ||
     (_go32_dpmi_lock_code(topOfFunctions,((char *)bottomOfFunctions-(char *)topOfFunctions))))
    return 0;
  return 1;
}

#ifdef _SB_INCLUDE_FILE_UTILS
sb_sample *sb_load_sample(char *fname, int format) {
  sb_sample *ss;
  int err, i;
  FILE *fp=fopen(fname,"rb");
  
  if(fp==NULL) {
    strcpy(sb_digi_error,"File not found!");
    return NULL;
  }

  ss=(sb_sample *)malloc(sizeof(sb_sample));
  if(ss==NULL) {
    strcpy(sb_digi_error,"Out of memory!");
    fclose(fp);
    return NULL;
  }
  if(_go32_dpmi_lock_data(ss,sizeof(sb_sample))) {
    strcpy(sb_digi_error,"Could not lock appropriate memory.");
    free(ss);
    fclose(fp);
    return NULL;
  }
  ss->callback=NULL;
  ss->left_volume=32;
  ss->right_volume=32;

  if(format==_SB_RAW)
    err=loadRaw(fp,ss);
  else if(format==_SB_WAV)
    err=loadWav(fp,ss);
  else if(format==_SB_VOC)
    err=loadVoc(fp,ss);
  else {
    strcpy(sb_digi_error,"Invalid file format!");
    fclose(fp);
    free(ss);
    return NULL;
  }

  fclose(fp);
  if(err==1) {                          /* Convert sample to SIGNED for mixing */
    for(i=0;i<ss->length;i++)           /* algorithm.                          */
      *(ss->data+i)=((BYTE)*(ss->data + i))-128;
    if(_go32_dpmi_lock_data(ss->data,ss->length)) {
      free(ss->data);
      free(ss);
      strcpy(sb_digi_error,"Could not lock appropriate memory.");
      return NULL;
    }
    return ss;
  }
  return NULL;
}

static int loadVoc(FILE *fp, sb_sample *ss) {
  char buf[20];
  BYTE currentBlk, temp, *tBuf;
  WORD firstBlock, version, id;
  DWORD blklen=0;

  fread(buf,1,19,fp);
  buf[19]=0;
  if((strcmp(buf,"Creative Voice File"))||(fgetc(fp)!=0x1A)) {
    strcpy(sb_digi_error,"Not a valid .VOC file!");
    return 0;
  }
  fread(&firstBlock,1,2,fp);
  fread(&version,1,2,fp);
  if(version>0x010A) {                  /* I don't have the specs for new .VOCs */
    strcpy(sb_digi_error,"Unsupported .VOC file version!");
    return 0;
  }
  fread(&id,1,2,fp);
  if(id!=(~version)+0x1234) {
    strcpy(sb_digi_error,"Not a valid .VOC file!");
    return 0;
  }
  fseek(fp,firstBlock,SEEK_SET);
  do {
    currentBlk=fgetc(fp);
    if(currentBlk!=0) {
      fread(&blklen,1,3,fp);
      switch(currentBlk) {
        case 1:
          temp=fgetc(fp);               /* Time-constant is useless to me */
          temp=fgetc(fp);
          if(temp!=0) {                 /* I can only handle raw 8-bit samples */
            strcpy(sb_digi_error,"Can't handle packed .VOCs!");
            return 0;
          }
          ss->length=blklen-2;
          ss->data=(BYTE *)malloc(ss->length);
          if(ss->data==NULL) {
            strcpy(sb_digi_error,"Out of memory!");
            return 0;
          }
          fread(ss->data,1,ss->length,fp);
          break;
        case 2:
          tBuf=(BYTE *)malloc(ss->length+blklen);
          if(tBuf==NULL) {
            strcpy(sb_digi_error,"Out of memory!");
            free(ss->data);
            return 0;
          }
          memcpy(tBuf,ss->data,ss->length);
          fread(tBuf+ss->length,1,blklen,fp);
          ss->length+=blklen;
          free(ss->data);
          ss->data=tBuf;
          break;
        default:
          while(blklen--) {
            fgetc(fp);
          };
          break;
      }
    }
  } while(currentBlk!=0);

  return 1;
}

static int loadWav(FILE *fp, sb_sample *ss) {
  BYTE dummydata[255];
  DWORD rlen,flen;
  WORD s_per_sec,b_per_sec,num_channels,tag;
  char riffid[5],waveid[5],fmtid[5],dataid[5];

  fread(riffid,1,4,fp);
  riffid[4] = 0;
  fread(&rlen,1,4,fp);
  fread(waveid,1,4,fp);
  waveid[4] = 0;
  if(strcmp(waveid,"WAVE")) {
    strcpy(sb_digi_error,"Not a valid .WAV file!");
    return 0;
  }

  fread(fmtid,1,4,fp);
  fmtid[4] = 0;
  fread(&flen,1,4,fp);
  if(flen>240)
    flen=240;

  fread(&tag,1,2,fp);
  fread(&num_channels,1,2,fp);
  fread(&s_per_sec,1,2,fp);
  fread(&b_per_sec,1,2,fp);
  fread(dummydata,1,(size_t)flen-8,fp);
  fread(dataid,1,4,fp);
  dataid[4] = 0;
  fread(&(ss->length),1,4,fp);

  ss->data=(BYTE *)malloc(ss->length+1);
  if(!ss->data) {
    strcpy(sb_digi_error,"Out of memory!");
    return 0;
  }

  fread(ss->data,1,ss->length,fp);
  return 1;
}

static int loadRaw(FILE *fp, sb_sample *ss) {
  if(fseek(fp,0,SEEK_END)==0) {
    if((ss->length=ftell(fp))!=-1) {
      if(fseek(fp,0,SEEK_SET)==0) {
        if((ss->data=(BYTE *)malloc(ss->length))!=NULL) {
          if(fread(ss->data,ss->length,1,fp)==1)
            return 1;
          else
            strcpy(sb_digi_error,"Error reading file!");
        }
        else
          strcpy(sb_digi_error,"Out of memory!");
      }
      else
        strcpy(sb_digi_error,"Seek error!");
    }
    else
      strcpy(sb_digi_error,"Tell error!");
  }
  else
    strcpy(sb_digi_error,"Seek error!");

  return 0;
}
#endif

void sb_free_sample(sb_sample *ss) {
  if(ss!=NULL) {
    free(ss->data);
    free(ss);
  }
}
