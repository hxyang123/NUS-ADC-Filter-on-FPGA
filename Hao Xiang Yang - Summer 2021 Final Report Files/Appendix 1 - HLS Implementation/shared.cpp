#include <iostream>
#include <fstream>
#include <sstream>

#include "shared.h"


bool readFile(const std::string fname,
		 std::vector<float> & results)
{

  std::cout << "Opening file: " <<fname << std::endl;
  std::ifstream in(fname.c_str());

  if(!in){
	  std::cerr << "Cannot open the File: " << fname << std::endl;
	  return false;
  }

  std::string str;
  while(std::getline(in, str)){
	  if(str.size() > 0){
		  std::stringstream ss(str);
		  float i;
		  while(ss >> i){
			  results.push_back(i);
			  if(ss.peek() == ',') ss.ignore();
		  }
	  }
  }

  std::cout << "Creating array for: " << fname << std::endl;
  //fptr = new float(results.size());
  //for(int i = 0; i < results.size(); i++){
	 // fptr = &results[0];
  //}
  return true;

}
