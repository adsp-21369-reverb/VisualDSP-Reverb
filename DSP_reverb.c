#include "DSP.h"
#include "DSP_lowpass.h"
#include "DSP_allpass.h"
#include "DSP_delay.h"
#include "DSP_mod_allpass.h"

// define size of delaylines for each module

//#define delayDivisor 4

//#define delaysize_predelay 0 // predelay not implemented
#define delaysize_lp1 2
#define delaysize_ap1 229
#define delaysize_ap2 173
#define delaysize_ap3 611
#define delaysize_ap4 447

#define delaysize_mod_ap1 1084
#define delaysize_delay1 7182
#define delaysize_lp2 2
#define delaysize_ap5 2903
#define delaysize_delay2 6000

#define delaysize_mod_ap2 1464
#define delaysize_delay3 6801
#define delaysize_lp3 2
#define delaysize_ap6 4284
#define delaysize_delay4 5101

#define delaysize_delay5 429
#define delaysize_delay6 4797
#define delaysize_delay7 3085
#define delaysize_delay8 3219
#define delaysize_delay9 3210
#define delaysize_delay10 302
#define delaysize_delay11 1719

#define delaysize_delay12 569
#define delaysize_delay13 5850
#define delaysize_delay14 1981
#define delaysize_delay15 4311
#define delaysize_delay16 3405
#define delaysize_delay17 540
#define delaysize_delay18 195

//#pragma file_attr("prefersMem=external")

//#pragma section("seg_sdram")
//#pragma section("seg_ext_dmda") // memory space
//#pragma section("seg_sdram_data") // object section
//#pragma section("seg_ext_data") // object section
//#pragma section("expand_ext_stack_and_heap") // object section

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ CREATE FILTER STRUCTS + global buffers ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// always add 1 sample for write/read index offset
// for mod. allpass add another (13*2) samples

// ~~~ MAIN INPUT PATH ~~~
//struct_delay1 predelay; // predelay not implemented

struct_lp lp1;
#pragma section ("seg_ext_data")
float buffer_lp1[delaysize_lp1+1];

struct_ap ap1;
#pragma section ("seg_ext_data")
float buffer_ap1[delaysize_ap1+1];

struct_ap ap2;
#pragma section ("seg_ext_data")
float buffer_ap2[delaysize_ap2+1];

struct_ap ap3;
#pragma section ("seg_ext_data")
float buffer_ap3[delaysize_ap3+1];

struct_ap ap4;
#pragma section ("seg_ext_data")
float buffer_ap4[delaysize_ap4+1];


// ~~~ LEFT TANK ~~~
struct_mod_ap mod_ap1;
#pragma section ("seg_ext_data")
float buffer_mod_ap1[delaysize_mod_ap1+1+(13*2)];

struct_delay delay1;
#pragma section ("seg_ext_data")
float buffer_delay1[delaysize_delay1+1];

struct_lp lp2;
#pragma section ("seg_ext_data")
float buffer_lp2[delaysize_lp2+1];

struct_ap ap5;
#pragma section ("seg_ext_data")
float buffer_ap5[delaysize_ap5+1];

struct_delay delay2;
#pragma section ("seg_ext_data")
float buffer_delay2[delaysize_delay2+1];


// ~~~ RIGHT TANK ~~~

struct_mod_ap mod_ap2;
#pragma section ("seg_ext_data")
float buffer_mod_ap2[delaysize_mod_ap2+1+(13*2)];

struct_delay delay3;
#pragma section ("seg_ext_data")
float buffer_delay3[delaysize_delay3+1];

struct_lp lp3;
#pragma section ("seg_ext_data")
float buffer_lp3[delaysize_lp3+1];

struct_ap ap6;
#pragma section ("seg_ext_data")
float buffer_ap6[delaysize_ap6+1];

struct_delay delay4;
#pragma section ("seg_ext_data")
float buffer_delay4[delaysize_delay4+1];


// ~~~ LEFT MAIN OUT ~~~
struct_delay delay5;
#pragma section ("seg_ext_data")
float buffer_delay5[delaysize_delay5+1];

struct_delay delay6;
#pragma section ("seg_ext_data")
float buffer_delay6[delaysize_delay6+1];

struct_delay delay7;
#pragma section ("seg_ext_data")
float buffer_delay7[delaysize_delay7+1];

struct_delay delay8;
#pragma section ("seg_ext_data")
float buffer_delay8[delaysize_delay8+1];

struct_delay delay9;
#pragma section ("seg_ext_data")
float buffer_delay9[delaysize_delay9+1];

struct_delay delay10;
#pragma section ("seg_ext_data")
float buffer_delay10[delaysize_delay10+1];

struct_delay delay11;
#pragma section ("seg_ext_data")
float buffer_delay11[delaysize_delay11+1];



// ~~~ RIGHT MAIN OUT ~~~
struct_delay delay12;
#pragma section ("seg_ext_data")
float buffer_delay12[delaysize_delay12+1];

struct_delay delay13;
#pragma section ("seg_ext_data")
float buffer_delay13[delaysize_delay13+1];

struct_delay delay14;
#pragma section ("seg_ext_data")
float buffer_delay14[delaysize_delay14+1];

struct_delay delay15;
#pragma section ("seg_ext_data")
float buffer_delay15[delaysize_delay15+1];

struct_delay delay16;
#pragma section ("seg_ext_data")
float buffer_delay16[delaysize_delay16+1];

struct_delay delay17;
#pragma section ("seg_ext_data")
float buffer_delay17[delaysize_delay17+1];

struct_delay delay18;
#pragma section ("seg_ext_data")
float buffer_delay18[delaysize_delay18+1];



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ CREATE GLOBAL VARIABLES ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float reverbIn;
float lp1_in, lp1_out;
//float predelay_in, predelay_out; // predelay not implemented
float ap1_in, ap1_out, ap2_in, ap2_out, ap3_in, ap3_out, ap4_in, ap4_out;
float tankIn, lTankIn, rTankIn;
float mod_ap1_in, mod_ap1_out, mod_ap2_in, mod_ap2_out;
float delay1_in, delay1_out, delay2_in, delay2_out;
float delay3_in, delay3_out, delay4_in, delay4_out;
float lp2_in, lp2_out, lp3_in, lp3_out;
float ap5_in, ap5_out, ap6_in, ap6_out;
static float lTankOut;
static float rTankOut;
float a, b, c, d, e, f;
float leftOut, rightOut;


void DSP_init(void)
{
	
	// init circulating sample variables
	lTankOut = 0;
	rTankOut = 0;
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~ INITIALIZE FILTER STRUCTS ~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//struct_delay_init(&delay1, predelaysize); // predelay not implemented
	
    lp_init(&lp1, buffer_lp1, delaysize_lp1, bandwidth);
	ap_init(&ap1, buffer_ap1, delaysize_ap1, g1);
    ap_init(&ap2, buffer_ap2, delaysize_ap2, g1);
    ap_init(&ap3, buffer_ap3, delaysize_ap3, g2);
    ap_init(&ap4, buffer_ap4, delaysize_ap4, g2);


    // ~~~ LEFT TANK ~~~
    mod_ap_init(&mod_ap1, buffer_mod_ap1, delaysize_mod_ap1, g3);
    delay_init(&delay1, buffer_delay1, delaysize_delay1);
    lp_init(&lp2, buffer_lp2, delaysize_lp2, (1-damping));
    ap_init(&ap5, buffer_ap5, delaysize_ap5, g4);
    delay_init(&delay2, buffer_delay2, delaysize_delay2);

    // ~~~ RIGHT TANK ~~~
    mod_ap_init(&mod_ap2, buffer_mod_ap2, delaysize_mod_ap2, g3);
    delay_init(&delay3, buffer_delay3, delaysize_delay3);
    lp_init(&lp3, buffer_lp3, delaysize_lp3, (1-damping));
    ap_init(&ap6, buffer_ap6, delaysize_ap6, g4);
    delay_init(&delay4, buffer_delay4, delaysize_delay4);

    // ~~~ LEFT MAIN OUT TAPS ~~~
    delay_init(&delay5, buffer_delay5, delaysize_delay5);
    delay_init(&delay6, buffer_delay6, delaysize_delay6);
    delay_init(&delay7, buffer_delay7, delaysize_delay7);
    delay_init(&delay8, buffer_delay8, delaysize_delay8);
    delay_init(&delay9, buffer_delay9, delaysize_delay9);
    delay_init(&delay10, buffer_delay10, delaysize_delay10);
    delay_init(&delay11, buffer_delay11, delaysize_delay11);

    // ~~~ RIGHT MAIN OUT TAPS ~~~
    delay_init(&delay12, buffer_delay12, delaysize_delay12);
    delay_init(&delay13, buffer_delay13, delaysize_delay13);
    delay_init(&delay14, buffer_delay14, delaysize_delay14);
    delay_init(&delay15, buffer_delay15, delaysize_delay15);
    delay_init(&delay16, buffer_delay16, delaysize_delay16);
    delay_init(&delay17, buffer_delay17, delaysize_delay17);
    delay_init(&delay18, buffer_delay18, delaysize_delay18);



}


// -------------------------------------------------
// ~~~ Main DSP Path ~~~
// -------------------------------------------------
void DSP_reverb(float *leftChannel, float *rightChannel)
{
	int i;

	for(i=0; i<NUM_SAMPLES/2; i++)
	{
		reverbIn = (leftChannel[i] + rightChannel[i])/2;

		//predelay->in = reverbIn;
		//delay(&predelay, reverbIn);
		//reverbIn = predelay->out;

		lp1_in = reverbIn;
		lp(&lp1, lp1_in);
		lp1_out = lp1.out;

							// test outputs:					
							//rightChannel[i] = lp1_out;
							//leftChannel[i] = lp1_out;

		// --------------------------------------------------------

		// INPUT DIFFUSION 1 : Nodes 13 - 20
		ap1_in = lp1.out;
		ap(&ap1, ap1_in);
		ap1_out = ap1.out;

		ap2_in = ap1.out;
		ap(&ap2, ap2_in);
		ap2_out = ap2.out;

		// INPUT DIFFUSION 2 : Nodes 15 - 22
		ap3_in = ap2.out;
		ap(&ap3, ap3_in);
		ap3_out = ap3.out;

		ap4_in = ap3.out;
		ap(&ap4, ap4_in);
		ap4_out = ap4.out;

							// test outputs:					
							//rightChannel[i] = ap4_out;
							//leftChannel[i] = ap4_out;


		// TANK INPUT
		tankIn = ap4.out;
		lTankIn = tankIn + rTankOut;
		rTankIn = tankIn + lTankOut;						

		// LEFT TANK : Nodes 23 - 39

		mod_ap1_in = lTankIn;
		mod_ap(&mod_ap1, mod_ap1_in);
		mod_ap1_out = mod_ap1.out;

							// test outputs:					
							//leftChannel[i] = mod_ap1.out;
							//rightChannel[i] = mod_ap1.out;


		//delay1_in = mod_ap1.in;			// bypass mod ap
		delay1_in = mod_ap1.out;  			// enable mod ap
		delay(&delay1, delay1_in);
		delay1_out = delay1.out;

		lp2_in = delay1.out;
		lp(&lp2, lp2_in);
		lp2_out = lp2.out;

		ap5_in = lp2.out * g5;
		ap(&ap5, ap5_in);
		ap5_out = ap5.out;

		delay2_in = ap5.out;
		delay(&delay2, delay2_in);
		delay2_out = delay2.out;

							// test outputs:					
							//leftChannel[i] = mod_ap1.out;
							//rightChannel[i] = mod_ap1.out;


		//-------------------------------------------------------------------------

		// RIGHT TANK : Nodes 46 - 63

		mod_ap2_in = rTankIn;
		mod_ap(&mod_ap2, mod_ap2_in);
		mod_ap2_out = mod_ap2.out;

		//delay3_in = mod_ap2.in;			// bypass mod ap
		delay3_in = mod_ap2.out;			// enable mod ap
		delay(&delay3, delay3_in);
		delay3_out = delay3.out;

		lp3_in = delay3.out;
		lp(&lp3, lp3_in);
		lp3_out = lp3.out;

		ap6_in = lp3.out * g5;
		ap(&ap6, ap6_in);
		ap6_out = ap6.out;

		delay4_in = ap6.out;
		delay(&delay4, delay4_in);
		delay4_out = delay4.out;

							// test outputs:					
							//leftChannel[i] = mod_ap2_out;
							//rightChannel[i] = mod_ap2_out;



		// TANK FEEDBACK => SEND TO TANK INPUT
		
		//lTankOut = 0;					// disable feedback to tank
		//rTankOut = 0;
		
		lTankOut = delay2.out * g5;		// enable feedback to tank
		rTankOut = delay4.out * g5;

							// test outputs:
							//leftChannel[i] = lTankOut;
							//rightChannel[i] = rTankOut;


		// TAP OUT
		a = delay1.tapout;
		b = ap5.tapout;
		c = delay2.tapout;
		d = delay3.tapout;
		e = ap6.tapout;
		f = delay4.tapout;

							// test outputs:
							//leftChannel[i] = f;
							//rightChannel[i] = f;


									
		// LEFT DELAY TAP OUT:
		delay(&delay5, a);
		delay(&delay6, a);
		delay(&delay7, b);
		delay(&delay8, c);
		delay(&delay9, d);
		delay(&delay10, e);
		delay(&delay11, f);
							
							// test outputs:
							//leftChannel[i] = delay10.out;
							//rightChannel[i] = delay11.out;

		// RIGHT DELAY TAP OUT:
		delay(&delay12, d);
		delay(&delay13, d);
		delay(&delay14, e);
		delay(&delay15, f);
		delay(&delay16, a);
		delay(&delay17, b);
		delay(&delay18, c);


		// TAP SUM:

		leftOut = delay5.out + delay6.out - delay7.out + delay8.out - delay9.out - delay10.out - delay11.out;
		rightOut = delay12.out + delay13.out - delay14.out + delay15.out - delay16.out - delay17.out - delay18.out;

		leftChannel[i] = leftOut;
		rightChannel[i] = rightOut;



		//-------------------------------------------------------------------------

	}

}
