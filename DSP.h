#ifndef __DSP__
#define __DSP__

#include "tt.h"
#include <stdlib.h>
#include <math.h>
#include "DSP_allpass.h"
#include "DSP_delay.h"
#include "DSP_lowpass.h"
#include "DSP_mod_allpass.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ DEFINE GLOBAL CONSTANTS ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define FS 48000                    // DACFS48 set in init1835viaSPI.c
#define BLKSZ NUM_SAMPLES/2       // NUM_SAMPLES set in ADDS_21369_EzKit.h
#define TWOPI 6.283185307

#define ADC_SCALE 1
#define DAC_SCALE 1

// Gains:

//#define Q12 12 // bit-shift operator for fixed/floating conversion
//#define SCALE 2^Q12 //4096 //2^Q for Q = 12; use for gain coeff multiplication

#define bandwidth 0.9995 // reverb bandwidth (0.9995)
#define g1 0.75             // input diffusion 1 (0.75)
#define g2 0.625            // input diffusion 2 (0.625)
#define g3 0.5              // decay diffusion 1 (0.7)
#define damping 0.0005     	// decay of higher frquencies (0.0005)
#define g4 0.5              // decay diffusion 2 (0.5)
#define g5 0.5              // decay (0.5)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ EXTERN VARIABLES ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//extern int loop_count;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ FUNCTION PROTOTYPES ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//extern void DSP_lfo(void);
//section ("seg_sdram") extern void DSP_init(void);
extern void DSP_init(void);
extern void DSP_reverb(float *leftChannel, float *rightChannel);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ FILTER STRUCTS ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//extern struct_delay1 predelay;
extern struct_lp lp1;
extern struct_ap ap1;
extern struct_ap ap2;
extern struct_ap ap3;
extern struct_ap ap4;

// ~~~ LEFT TANK ~~~
extern struct_mod_ap mod_ap1;
extern struct_delay delay1;
extern struct_lp lp2;
extern struct_ap ap5;
extern struct_delay delay2;

// ~~~ RIGHT TANK ~~~
extern struct_mod_ap mod_ap2;
extern struct_delay delay3;
extern struct_lp lp3;
extern struct_ap ap6;
extern struct_delay delay4;

// ~~~ LEFT MAIN OUT ~~~
extern struct_delay delay5;
extern struct_delay delay6;
extern struct_delay delay7;
extern struct_delay delay8;
extern struct_delay delay9;
extern struct_delay delay10;
extern struct_delay delay11;

// ~~~ RIGHT MAIN OUT ~~~
extern struct_delay delay12;
extern struct_delay delay13;
extern struct_delay delay14;
extern struct_delay delay15;
extern struct_delay delay16;
extern struct_delay delay17;
extern struct_delay delay18;


#endif
