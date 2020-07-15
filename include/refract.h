#ifndef REFRACT_H
#define REFRACT_H
#include <Eigen/Core>
// Reflect an incoming ray into an out going ray
//
// Inputs:
//   in  incoming _unit_ ray direction
//   n  surface _unit_ normal about which to reflect
// Returns outward _unit_ ray direction
// c2 is index of refraction of object

bool refract(const Eigen::Vector3d & in, const Eigen::Vector3d & n, Eigen::Vector3d & ref_ray ,const double c1, const double c2);
#endif 
