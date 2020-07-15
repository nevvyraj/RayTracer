#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "Light.h"
#include <Eigen/Core>
class SpotLight : public Light
{
  public:
    Eigen::Vector3d p;

	//cutoff angle of light in degrees
	float cutoff_angle;    

	//direction of spot light towards the scene
	Eigen::Vector3d d;

    // Given a query point return the direction _toward_ the Light.
    //
    // Input:
    //   q  3D query point in space
    // Outputs:
    //    d  3D direction from point toward light as a vector.
    //    max_t  parametric distance from q along d to light (may be inf)
	void direction(
      const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const;

	bool doesLightInteract(const Eigen::Vector3d & q) const;
	
	//return direction vector from query q to light at point p
	Eigen::Vector3d getLightDir(const Eigen::Vector3d & q) const;
};
#endif

