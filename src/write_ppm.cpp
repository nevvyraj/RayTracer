#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code from computer-graphics-raster-images
  ////////////////////////////////////////////////////////////////////////////

  assert(
    (num_channels == 3 || num_channels ==1 ) &&
    ".ppm only supports RGB or grayscale images");
  
  std::ofstream ppm;
  ppm.open(filename);

  if (!ppm.is_open()){
	return false;
  }

  if (num_channels == 3){
  	ppm << "P3\n";
  }
  else if (num_channels == 1){
	ppm << "P2\n";
  }
  ppm << width << " " << height << "\n";
  ppm << "255\n";

  
  for (int i = 0 ; i < data.size(); i ++){
  	ppm << (unsigned short)data[i] << " ";
  }

 
  ppm.close();
  
  return true;
	

}
