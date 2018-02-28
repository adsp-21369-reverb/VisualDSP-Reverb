///////////////////////////////////////////////////////////////////////////////////////
//NAME:     main.c (Block-based Talkthrough)
//DATE:     7/29/05
//PURPOSE:  Talkthrough framework for sending and receiving samples to the AD1835.
//
//USAGE:    This file contains the main routine calls functions to set up the talkthrough
//          routine. It receives an input signal from the ADC via SPORT0A and outputs to
//          DAC's via SPORT1A, SPORT1B, SPORT2A, and SPORT2B.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "tt.h"
#include "DSP.h"
#include <stdfix.h>
#include <float.h>

//#pragma section("seg_ext_data")
//float buffer1[10000]; // => check location inside memory map xml file

void main(void)
{

    //Initialize PLL to run at CCLK= 331.776 MHz & SDCLK= 165.888 MHz.
    //SDRAM is setup for use, but cannot be accessed until MSEN bit is enabled
    InitPLL_SDRAM();

    //SetParameters();
    DSP_init();
    
    // Setting up IRQ0 and IRQ1
    SetupIRQ01();

    // Need to initialize DAI because the sport signals need to be routed
    InitSRU();

    // This function will configure the codec on the kit
    Init1835viaSPI();

    interrupt (SIG_SP0, TalkThroughISR);
    interrupt (SIG_IRQ0, Irq0ISR);
    interrupt (SIG_IRQ1, Irq1ISR);

    // Finally setup the sport to receive / transmit the data
    InitSPORT();

    
    // Be in infinite loop and do nothing until done.
    for(;;)
    {
     while(blockReady)
          processBlock(src_pointer[int_cntr]);
    }

}
