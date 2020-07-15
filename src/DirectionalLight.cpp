#include "DirectionalLight.h"
#include <limits>

void DirectionalLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{

	d = (-1*this->d);
	//d.normalize();	
	max_t = std::numeric_limits<double>::infinity();

}

bool DirectionalLight::doesLightInteract(const Eigen::Vector3d & q) const
{
	return true;
}


