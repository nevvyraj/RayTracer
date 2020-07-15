#include "PointLight.h"
#include <iostream>

void PointLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////

	d = p - q; //direction from point to light
	d.normalize();
	//using x direction to find max_t
	max_t = (p(0) - q(0)) / d(0);	
	
	//testing to make t is equal for all components
	//double ty = (p(1) - q(1))/d(1);
	//double tz = (p(2) - q(2))/d(2);
	//assert (max_t == ty && max_t == tz && "One of the t's are not equal");
	
}


bool PointLight::doesLightInteract(const Eigen::Vector3d & q) const
{
	return true;
}
