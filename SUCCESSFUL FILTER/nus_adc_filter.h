
#ifndef _NUS_ADC_FILTER_H
#define _NUS_ADC_FILTER_H

// Limits

//#define float double

#define N_INT_PARAM 100
#define PI 3.14159265358979



float integral(float time, float a, float b, int n, const int cut_off_freq);
float sinc_f(float x, const int cut_off_freq);


void nus_adc_f_2(volatile float *mem,
              int X_offset,
              int Qt_offset,
			  int yout_offset,
			  int X_num,
			  int Qt_num,
			  int yout_num,
	        const int cut_off_freq);
#endif
