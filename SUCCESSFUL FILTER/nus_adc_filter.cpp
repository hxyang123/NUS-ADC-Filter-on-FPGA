#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "nus_adc_filter.h"

void nus_adc_f_2(volatile float *mem,
              int X_offset,
              int Qt_offset,
	      int yout_offset,
      	      int X_num,
   	      int Qt_num,
	      int yout_num,
	      const int cut_off_freq)
{
#pragma HLS INTERFACE m_axi depth=35000 port=mem
#pragma HLS INTERFACE s_axilite port=X_offset bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=Qt_offset bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=yout_offset bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=X_num bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=Qt_num bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=yout_num bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=cut_off_freq bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS

	//reset values
	if(X_num >= Qt_num){
			X_num = Qt_num - 1;
	}
	if(yout_num > X_num){
		yout_num = X_num;
	}


	//set values
	float time;
	//float sum = 0;

	//set pointers to memories
	volatile float *X_mem = mem + X_offset;
	volatile float *Qt_mem = mem + Qt_offset;
	volatile float *yout_mem = mem + yout_offset;

	float X[3000];
	float Qt[3001];
	float yout[1200];

	for(int i=0; i < yout_num; i++){
		yout[i] = 0;
	}

	memcpy(X, (const float*) X_mem, X_num*sizeof(float));
	memcpy(Qt, (const float*) Qt_mem, Qt_num*sizeof(float));


	int j = 1;
  while(j < yout_num+1){


	//time = (float) j/X_num*(*(Qt_mem + Qt_num-1)-*Qt_mem) + *Qt_mem;
	time = (float) j/X_num*(Qt[Qt_num-1] - Qt[0]) + Qt[0];

	for(int i=0; i < X_num; i++){
		//sum += *(X_mem+i)*integral(time, *(Qt_mem+i), *(Qt_mem+i+1), N_INT_PARAM, cut_off_freq);

		yout[j-1] += X[i] *integral(time, Qt[i], Qt[i+1], N_INT_PARAM, cut_off_freq);
	}

	//*(yout_mem++) = (float) sum;


	//std::cout << *(yout_mem-1) << std::endl;
	//sum = 0;

	j++;
  }

  memcpy((float*) yout_mem, yout, yout_num*sizeof(float));

}


float sinc_f(float x, const int cut_off_freq){

	if (x==0)
	        return 1;
	return sin(2*PI*cut_off_freq*x)/(PI*x);
}

//Riemann sum
float integral(float time, float a, float b, int n, const int cut_off_freq) {
	float step = (b - a) / n;  // width of each small rectangle
	float area = 0;  // signed area
  for (int i = 0; i < n; i++) {
	  area += sinc_f((a + (i + 0.5) * step) - time, cut_off_freq) * step; // sum up each small rectangle
  } 
  return area; 
}



