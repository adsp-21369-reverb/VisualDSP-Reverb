#include "tt.h"
#include <stdlib.h>
#include "DSP.h"
//#include "DSP_lfo.h"
#include "DSP_mod_allpass.h"
#include <math.h>
//#pragma file_attr("prefersMem=external")

void mod_ap_init(struct_mod_ap *data, float *buffer_mod_ap, int delaysize, float gain)
{
        data->in = 0;
        data->out = 0;
        data->tapout = 0;
        data->interpolIn = 0;
        data->interpolDelay = 0;
        data->interpolOut = 0;
        data->g = gain;
        data->eta = 0.1; //-0.05
        data->rIndex = 1; // 2nd oldest sample
        //data->rIndexMinus1 = 1; // oldest sample
        data->wIndex = 0; // newest sample
        data->delaysize = (delaysize+1) + (13*2); // 13 samples for excursion overhead
        data->varDelaysize = 0;
        data->buffer = buffer_mod_ap; // pointer to buffer array

        //data->buffer = (float *)calloc(data->delaysize, sizeof(float));
        //data->buffer = (float *)malloc(data->delaysize * sizeof(float));
        data->excursion_int = 0;
        data->excursion_float = 0;
        data->lfoIndex = 0;

        data->tau = 0.95;
        data->x_new = 0;
        data->x_old = 0;
        data->y_new = 0;
        data->y_old = 0;

        // clear buffer at init, commented out due to calloc instead of malloc
        int i;
        for(i=0; i < data->delaysize; i++)
        data->buffer[i] = 0;

}

void mod_ap(struct_mod_ap *data, float sig)
{

        data->excursion_float = 0;	// deactivate modulated delay line
        
		//data->excursion_float = 13*(cos(data->lfoIndex * TWOPI / (FS))-1); // excursion [0,-26]
        data->excursion_int = (int) (ceil(data->excursion_float));
        data->tau = ((float) data->excursion_int) - data->excursion_float;
        data->varDelaysize = data->delaysize + data->excursion_int;	
	
		// ----- Mod. APF with nested allpass interpolation:
        data->in = sig;
        data->x_new = data->buffer[data->rIndex];
        // Simple allpass interpolator
        data->y_new = (1-data->tau)*(data->x_new - data->y_old) + data->x_old;
        // Warped allpass interpolator
        //data->y_new = ((1-data->tau)/(1+data->tau))*(data->x_new - data->y_old) + data->x_old;
        data->tapout = sig + (data->y_new * data->g);
        data->out = data->y_new - (data->tapout * data->g);
        data->buffer[data->wIndex] = data->tapout;
        
        data->x_old = data->x_new;
        data->y_old = data->y_new;
        // ----- Code end
		
        // ----- Mod. APF with nested linear interpolation:
        /*data->in = sig;
        data->x_new = data->buffer[data->rIndex];
        data->y_new = ((1-data->tau)*data->x_new) + ((data->tau) * data->x_old);
        data->tapout = sig + (data->y_new * data->g);
        data->out = data->y_new - (data->tapout * data->g);
        data->buffer[data->wIndex] = data->tapout;
        
        data->x_old = data->x_new;*/
        // ----- Code end
        
		// ----- Mod. APF w/o Interpolation:
		/*data->in = sig;
        data->tapout = sig + (data->buffer[data->rIndex] * data->g);
        data->out = data->buffer[data->rIndex] - (data->tapout * data->g);
        data->buffer[data->wIndex] = data->tapout;*/
		// ----- Old Code end
                

        //data->rIndexMinus1++;
        data->rIndex++;
        data->wIndex++;
        data->lfoIndex++;

        /*if (data->rIndexMinus1 >= data->varDelaysize - 1)
        data->rIndexMinus1=0;*/

        if (data->rIndex >= data->varDelaysize)
        data->rIndex=0;

        if (data->wIndex >= data->varDelaysize)
        data->wIndex=0;

        if(data->lfoIndex >= (FS))
        data->lfoIndex=0;

}
