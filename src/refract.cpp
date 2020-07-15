#include <Eigen/Core>
#include <iostream>
#include <cmath>

#include "refract.h"
#define PRINT_VEC(d) (std::cout << #d << ": " << d(0) << ", " << d(1) << ", " << d(2) << "\n")

bool refract(const Eigen::Vector3d & in, const Eigen::Vector3d & n, Eigen::Vector3d & ref_ray, const double c1, const double c2)
{

	double ratio = c1/c2;

	//Eigen::Vector3d l = in;
	Eigen::Vector3d l = in.normalized();
	double dn = l.dot(n);


	double under_root = 1.0 - ((ratio*ratio) * (1.0 - (dn*dn)));
	if (under_root < 0.0){
		return false;
	}

	
	ref_ray = (ratio)*(l - n*(dn)) - n*sqrt(under_root);
	ref_ray.normalize();
	
	//PRINT_VEC(in);	
	//PRINT_VEC(ref_ray);	

	return true;	

}
