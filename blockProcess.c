#include "tt.h"
#include "DSP.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
//#include <stdfix.h>
//#include <float.h>

#define mix 0.25 // 0.0 for dry signal, 1.0 for wet signal

static int loop_count = 0;

#pragma misra_func(time)
clock_t start_t, end_t, total_t;
float clocks, timer;

void Block_Fixed_To_Float( int * Fixed_In, float * Float_Out_L, float * Float_Out_R );
void Block_Float_To_Fixed( int * Fixed_Out, float * Float_In_L, float * Float_In_R );

void processBlock(unsigned int *block_ptr)
{
    int i;
    static float leftChannel[NUM_SAMPLES/2];
    static float rightChannel[NUM_SAMPLES/2];
    float leftDry[NUM_SAMPLES/2];
    float rightDry[NUM_SAMPLES/2];
        
    start_t = clock();	// start timer
   	        
    //Clear the Block Ready Semaphore
    blockReady = 0;
    
    //Set the Processing Active Semaphore before starting processing
    isProcessing = 1;
	
    loop_count++; // count loops until sdram error
    
    
    Block_Fixed_To_Float((int *) block_ptr, leftChannel, rightChannel);
    
    for(i=0; i<NUM_SAMPLES/2;i++)
    {
    	leftDry[i] = leftChannel[i];
    	rightDry[i] = rightChannel[i];
    }
    
    
    DSP_reverb(leftChannel, rightChannel);
    
    
    for(i=0; i<NUM_SAMPLES/2;i++)
    {
    	leftChannel[i] = leftChannel[i]*mix + leftDry[i]*(1-mix);
    	rightChannel[i] = rightChannel[i]*mix + rightDry[i]*(1-mix);
    }
    
    Block_Float_To_Fixed((int *) block_ptr, leftChannel, rightChannel);
    
	

    //Clear the Processing Active Semaphore after processing is complete
    isProcessing = 0;
    
    clocks = CLOCKS_PER_SEC;
    end_t = clock();	// end timer
    total_t = end_t - start_t;	// compute process cycles
	timer = ((double) (total_t))/CLOCKS_PER_SEC;	// compute process time
    
}

void Block_Fixed_To_Float( int * Fixed_In, float * Float_Out_L, float * Float_Out_R )
	{
	int i;
	#pragma SIMD_for
	for (i=0;i<NUM_SAMPLES/2;i++)
	{
		Float_Out_L[i] = ((float) (Fixed_In[2*i]<<8)) * (1.0/2147483648.0);
		Float_Out_R[i] = ((float) (Fixed_In[2*i+1]<<8)) * (1.0/2147483648.0);
	}
}

void Block_Float_To_Fixed( int * Fixed_Out, float * Float_In_L, float * Float_In_R )
{
	int i;
	#pragma SIMD_for
	for (i=0;i<NUM_SAMPLES/2;i++)
	{
		Fixed_Out[2*i] = ((int) (2147483648.0*Float_In_L[i]))>>8;
		Fixed_Out[2*i+1] = ((int) (2147483648.0*Float_In_R[i]))>>8;
	}
}
