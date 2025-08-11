#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "nus_adc_filter.h"
#include <stdlib.h>
#include "shared.h"
using namespace std;

int main(int argc, char ** argv)
{
	float * mem;
  
  int retval = 0;

  //string imageDir = "matlab_data";
  string imageDir = "matlab_data_10000HZ";

  if(argc == 2)
    imageDir = argv[1];
    cout << "Using test " << imageDir <<  endl;

  cout << "Starting NUS_ADC_FILTER Test " << endl;

  vector<float> X_inputs;
  vector<float> Qt_inputs;
  vector<float> yout_gold_outputs;

  //bool X_params = readFile(imageDir + "/input_X.txt", X_inputs);
  //bool Qt_params = readFile(imageDir + "/input_Qt.txt", Qt_inputs);
  //bool output_params = readFile(imageDir + "/output_yout.txt", yout_gold_outputs);

  bool X_params = readFile(imageDir + "/X_10000HZ.txt", X_inputs);
  bool Qt_params = readFile(imageDir + "/Qt_10000HZ.txt", Qt_inputs);
  bool output_params = readFile(imageDir + "/yout_10000HZ.txt", yout_gold_outputs);


  if (!X_params||!Qt_params||!output_params)
    {
      cerr << "Problem with input files\n";
      retval = 1;
    }
    else
    {

    vector<float> yout_outputs(yout_gold_outputs.size());
   
    //int cut_off_freq = 1000000;

    int cut_off_freq = 10000;

    cout << "Begin Test\n"
       << "Cut Off Freq: " << cut_off_freq << "Hz" << endl
       << "Num X Inputs: " << X_inputs.size() << endl
       << "Num Qt Inputs: " << Qt_inputs.size() << endl
       << "Num Outputs: " << yout_gold_outputs.size() << endl;
 

    //parameters

    int X_offset = 0;
    int Qt_offset = X_inputs.size();
    int yout_offset = Qt_offset + Qt_inputs.size();


    //how much of each will the filter be using to generate outputs
    int X_num = 1000;
    int Qt_num = 1001;
    int yout_num = 400;

    vector<float> dynamic_mem;
    dynamic_mem.reserve(X_inputs.size() + Qt_inputs.size() + yout_outputs.size());
    dynamic_mem.insert(dynamic_mem.end(), X_inputs.begin(), X_inputs.end());
    dynamic_mem.insert(dynamic_mem.end(), Qt_inputs.begin(), Qt_inputs.end());
    dynamic_mem.insert(dynamic_mem.end(), yout_outputs.begin(), yout_outputs.end());

    mem = &dynamic_mem[0];

    // Run filter
	nus_adc_f_2(mem, X_offset, Qt_offset, yout_offset, X_num, Qt_num, yout_num, cut_off_freq);


	float *yout = mem + yout_offset;

    // Check outputs
    float total = 0.0f;
    //std::cout << "Printing yout[i] and gold yout[i]: \n";
    for (int i = 0; i < yout_num; i++)
    {
    	//std::cout << yout[i] << std::endl;
      float err = fabs(yout[i] - yout_gold_outputs[i]);
      total += err*err;
    }
    float avg_error = total/(yout_num);
    cout << "Mean Square Error " << avg_error << endl;
    }
  return retval;
}
