#ifndef __DSP_ALLPASS__
#define __DSP_ALLPASS__

typedef struct
{
  float in;
  float out;
  float tapout;
  float g;
  int rIndex;
  int wIndex;
  int delaysize;
  float *buffer;
} struct_ap;


void ap_init(struct_ap *data, float *buffer_ap, int delaysize, float gain);
void ap(struct_ap *data, float sig);

#endif
