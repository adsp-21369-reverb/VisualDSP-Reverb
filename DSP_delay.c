#include "tt.h"
#include <stdlib.h>
#include "DSP.h"
#include "DSP_delay.h"
//#pragma file_attr("prefersMem=external")

void delay_init(struct_delay *data, float *buffer_del, int delaysize)
{

		data->in = 0;
        data->out = 0;
        data->tapout = 0;   
        data->rIndex = 1; // points at first+1 buffer element     
        data->wIndex = 0; // points at first buffer element
        data->delaysize = delaysize+1;    // i.e. z => elements of {0,1,2,...,z-1}
        data->buffer = buffer_del; // pointer to buffer array
        
        //data->buffer = (float *)calloc(data->delaysize, sizeof(float));
		//data->buffer = (float *)malloc(data->delaysize * sizeof(float));
        
        // clear buffer at init, commented out due to calloc instead of malloc
        int i;
        for(i=0; i < data->delaysize; i++)
        data->buffer[i] = 0;

}


void delay(struct_delay *data, float sig)

{

        data->in = sig;
        data->tapout = sig;
        data->out = data->buffer[data->rIndex];
        data->buffer[data->wIndex] = sig;
        
        data->rIndex++;
        data->wIndex++;

        if(data->rIndex >= data->delaysize)
        data->rIndex=0;

        if(data->wIndex >= data->delaysize)
        data->wIndex=0;

}
