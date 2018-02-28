#ifndef __DSP_DELAY__
#define __DSP_DELAY__

typedef struct
{
  float in;
  float out;
  float tapout;
  int rIndex;
  int wIndex;
  int delaysize;
  float *buffer;
} struct_delay;


extern void delay_init(struct_delay *data, float *buffer_del, int delaysize);
extern void delay(struct_delay *data, float sig);

#endif
