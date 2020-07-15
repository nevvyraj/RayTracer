#include "viewing_ray.h"

#include<iostream>

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

	double image_u = (camera.width * ((double)j + 0.5) / width) - camera.width/2.0; 	

	//tranformation on slide assumes image origin at bottom left, assignment assumes top left
	double image_v = -(camera.height * ((double)i + 0.5) / height) + camera.height/2.0; 	

	//following perspective view from textbook
	ray.origin = camera.e;
	
	ray.direction = image_u*camera.u + image_v*camera.v - camera.d*camera.w;
}

