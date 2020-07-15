#include "first_hit.h"
#include <limits>

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////
	
	hit_id = -1;
	double closest = std::numeric_limits<double>::infinity();
	
	Eigen::Vector3d normal; //normal to POI for each object, used for each intersection test
	double t_tmp = 1; //t used in each check

	for (int i = 0; i < objects.size(); i++){

		if (objects[i]->intersect(ray, min_t,t_tmp,normal)){
			if (t_tmp < closest && t_tmp > 0.0){
				closest = t_tmp;
				hit_id = i;
				n = normal;
				t = t_tmp;
			}	
		}

	}

	if (hit_id == -1){
		return false;
	}

	return true;

}

