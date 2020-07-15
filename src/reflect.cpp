#include <Eigen/Core>
#include <iostream>

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{

	Eigen::Vector3d out;
	Eigen::Vector3d l = in;

	l.normalize();

	out = (l ) - (2*n) * (n.dot(l));
	//	std::cout << "Ray: " << in << "\n\n";
	//	std::cout << "normal: " << n << "\n\n";
	//	std::cout << "Reflected_ray: " << out << "\n\n";
	out.normalize();
	return out;

}
