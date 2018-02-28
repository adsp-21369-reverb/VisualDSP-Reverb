#ifndef __DSP_MOD_ALLPASS__
#define __DSP_MOD_ALLPASS__


typedef struct
{
  float in;
  float out;
  float tapout;
  float interpolIn;
  float interpolDelay;
  float interpolOut;
  float g;
  float eta;
  int rIndex;
  //int rIndexMinus1;
  int wIndex;
  int delaysize;
  int varDelaysize;
  float *buffer;
  int lfoIndex;
  int excursion_int;
  float excursion_float;
  float tau;
  float x_new;
  float x_old;
  float y_new;
  float y_old;
} struct_mod_ap;


void mod_ap_init(struct_mod_ap *data, float *buffer_mod_ap, int delaysize, float gain);
void mod_ap(struct_mod_ap *data, float sig);

#endif
