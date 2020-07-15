#include "Sphere.h"
#include "Ray.h"

#include <cmath>
#include <iostream>

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////

	double a = ray.direction.transpose().dot(ray.direction);
	double b = (2*ray.direction).transpose().dot(ray.origin - center);
	
	double r = radius;
	double c = (ray.origin - center).transpose().dot(ray.origin - center) - (r*r);
	
//	std::cout << ray.direction << " \n--\n " << ray.origin << " \n---\n " << center << "\n";
//	std::cout << "a: " << a << " b: " << b << " c: " << c << "\n";

	double discriminant = (b * b) - (4*a*c);
//	std::cout << "d: " << discriminant <<"\n";
	if (discriminant < 0.0){
		//no roots
		//std::cout << "not hit\n";
		return false;
	}	

	
	double t1 = -b + std::sqrt(discriminant);
	t1 = t1 / (2*a);

	double t2 = -b - std::sqrt(discriminant);
	t2 = t2 / (2*a);


	//check incase we are inside the sphere, therefore only point to consider is the one beyond min_t
	if (t1 <= min_t && t2 > min_t){
		t = t2;
	}
	else if (t2 <= min_t && t1 > min_t){
		t = t1;
	}
	else{
		t = t1 < t2 ? t1 : t2;
	}
	
	//std::cout << t1 << " " << t2 << "\n";
	//std::cout << t << std::endl << (t >= min_t) <<  "\n---------\n";	
	
	Eigen::Vector3d point = ray.origin + t*ray.direction;	

	n = (point - center);
	n.normalize();

	return t >= min_t;

}

