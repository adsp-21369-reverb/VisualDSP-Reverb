#include "tt.h"
#include <stdlib.h>
#include "DSP.h"
#include "DSP_allpass.h"
//#pragma file_attr("prefersMem=external")

void ap_init(struct_ap *data, float *buffer_ap, int delaysize, float gain)
{
        data->in = 0;
        data->out = 0;        
        data->tapout = 0;
        data->g = gain;
        data->rIndex = 1; // rIndex is running just 1 sample ahead in front of wIndex, reading the oldest sample
        data->wIndex = 0; // running 1 sample after rIndex, overwriting the oldest sample
        data->delaysize = delaysize+1;
        data->buffer = buffer_ap; // pointer to buffer array
        
        //data->buffer = (float *)calloc(data->delaysize, sizeof(float));
		//data->buffer = (float *)malloc(data->delaysize * sizeof(float));
        
        // clear buffer at init, commented out due to calloc instead of malloc
        int i;
        for(i=0; i < data->delaysize; i++)
        data->buffer[i] = 0;

}

void ap(struct_ap *data, float sig)
{

        data->in = sig;
        data->tapout = sig - (data->buffer[data->rIndex] * data->g);
        data->out = data->buffer[data->rIndex] + (data->tapout * data->g);
        data->buffer[data->wIndex] = data->tapout;
        
        data->rIndex++;
        data->wIndex++;

        if (data->rIndex >= data->delaysize)
        data->rIndex=0;

        if (data->wIndex >= data->delaysize)
        data->wIndex=0;

}

