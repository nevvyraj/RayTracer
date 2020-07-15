#include "Triangle.h"
#include "Ray.h"

#include <Eigen/Dense> //for matrix determinant

#include <iostream>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  //return false;
  ////////////////////////////////////////////////////////////////////////////

	Eigen::Vector3d vertex_a = std::get<0>(corners);
	Eigen::Vector3d vertex_b = std::get<1>(corners);
	Eigen::Vector3d vertex_c = std::get<2>(corners);

	double xa = vertex_a(0);
	double ya = vertex_a(1);
	double za = vertex_a(2);

	double xb = vertex_b(0);
	double yb = vertex_b(1);
	double zb = vertex_b(2);
	
	double xc = vertex_c(0);
	double yc = vertex_c(1);
	double zc = vertex_c(2);


	double xe = ray.origin(0);
	double ye = ray.origin(1);
	double ze = ray.origin(2);
	
	double xd = ray.direction(0);
	double yd = ray.direction(1);
	double zd = ray.direction(2);


	Eigen::Matrix3d A;
	A << (xa-xb), (xa-xc), xd,
	  	(ya-yb), (ya-yc), yd,
		(za-zb), (za-zc), zd;

	double det_a = A.determinant();

	Eigen::Matrix3d beta_matrix;
	Eigen::Matrix3d gamma_matrix;
	Eigen::Matrix3d t_matrix;
	
	beta_matrix << (xa-xe), (xa-xc), xd,
					(ya-ye), (ya-yc), yd,
					(za-ze), (za-zc), zd;

	gamma_matrix << (xa-xb), (xa-xe), xd,
				 	(ya-yb), (ya-ye), yd,
					(za-zb), (za-ze), zd;

	t_matrix << (xa-xb), (xa-xc), (xa-xe),
			   (ya-yb), (ya-yc), (ya-ye),
			   (za-zb), (za-zc), (za-ze);


	double beta = beta_matrix.determinant() / det_a;
	double gamma = gamma_matrix.determinant() / det_a;
	t = t_matrix.determinant() / det_a;

	if (t < min_t) {
		return false;
	}

	if (gamma < 0.0 || gamma > 1.0){
		return false;
	}
	
	if (beta < 0.0 || beta > 1.0 - gamma){
		return false;
	}


	//calculate normal, cross product of two sides from triangle
	Eigen::Vector3d side_a = vertex_b - vertex_a;
	Eigen::Vector3d side_b = vertex_c - vertex_a;

	n = side_a.cross(side_b);
	n.normalize();

	return true;


}	


