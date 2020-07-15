#ifndef MATERIAL_H
#define MATERIAL_H
#include <Eigen/Core>

// Blinn-Phong Approximate Shading Material Parameters
struct Material
{
	// Ambient, Diffuse, Specular, Mirror Color
	Eigen::Vector3d ka,kd,ks,km;

	Eigen::Vector3d kt; //transmissive color

	double ior; //index of refraction

	bool is_glossy = false; //is reflection a glossy one?

  	// Phong exponent
  	double phong_exponent;
};
#endif
