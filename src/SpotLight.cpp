#include "SpotLight.h"
#include <iostream>
#include <cmath>

#define PRINT_VEC(d) (std::cout << #d << ": " << d(0) << ", " << d(1) << ", " << d(2) << "\n")

Eigen::Vector3d SpotLight::getLightDir(const Eigen::Vector3d & q) const
{
	return p - q;
}

void SpotLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{

	Eigen::Vector3d light_dir;
	light_dir = getLightDir(q); //direction from point to light
	light_dir.normalize();
	
	//using x direction to find max_t
	max_t = (p(0) - q(0)) / light_dir(0);	
	
	d = (-1*this->d);
}

bool SpotLight::doesLightInteract(const Eigen::Vector3d & q) const
{
	//convert angle to rad
	double rad_angle = cutoff_angle * M_PI / 180.0;
	double threshold = std::cos(rad_angle);

	Eigen::Vector3d light_dir = getLightDir(q);
	light_dir = -1*light_dir;
	light_dir.normalize();



	Eigen::Vector3d dir = this->d;

	double dot = dir.dot(light_dir);

	return dot > threshold;
	
}
