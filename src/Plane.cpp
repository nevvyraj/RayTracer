#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////


	double denominator = normal.transpose().dot(ray.direction);

	if (denominator == 0.0){
		//ray is parallel to plane
		return false;
	}

	double numerator = (point - ray.origin).dot(normal.transpose());

	t = numerator / denominator;

	n = normal;
	n.normalize();

	return t >= min_t;

}

