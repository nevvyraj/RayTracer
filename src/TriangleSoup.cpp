#include "TriangleSoup.h"
#include "Ray.h"
// Hint
#include "first_hit.h"

bool TriangleSoup::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////

	double closest = std::numeric_limits<double>::infinity();
	double t_tmp = -1.0;
	bool found = false;
	Eigen::Vector3d normal;

	for (int i = 0; i < triangles.size(); i++){

		if (triangles[i]->intersect(ray,min_t,t_tmp,normal)){
			if (t_tmp < closest && t_tmp >= min_t){
				closest = t_tmp;
				t = t_tmp;
				n = normal;
				found = true;
			}
		}

	}


	if (!found){
		return false;
	}

	return true;

}



