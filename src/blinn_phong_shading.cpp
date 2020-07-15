#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>

#include <cmath>

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
	//return color
	Eigen::Vector3d rgb(0,0,0);
	
	Eigen::Vector3d poi; //intersection point	
	poi = ray.origin + t*ray.direction;

	//light vector
	Eigen::Vector3d l;
	double light_src_max_t;

	//blinn vectors - unit length
	Eigen::Vector3d h; //half vector
	Eigen::Vector3d v; //view vector from poi to ray origin

	v = ray.origin - poi;
	v.normalize();

	//shadow
	const double epsilon = 1e-6;
	double shadow_t = 0;
	int shadow_hit_id;
	Eigen::Vector3d shadow_n;

	Eigen::Vector3d ambient_col(0,0,0);
	Eigen::Vector3d diffuse_col(0,0,0);
	Eigen::Vector3d spec_col(0,0,0);
	Eigen::Vector3d shadow_col(0,0,0);
	
	std::shared_ptr<Material> material = objects[hit_id]->material;	
	ambient_col = material->ka * 0.1;

	auto max = [](double a, double b){return a > b ? a : b;}; 	
   	for (int i = 0; i < lights.size(); i++){
		if(!lights[i]->doesLightInteract(poi)){
			continue; //light doesn't interact with object at point "poi"
		}
		
		Eigen::Vector3d scalar_diffuse = (material->kd.array() * lights[i]->I.array()).matrix();
		Eigen::Vector3d scalar_spec = (material->ks.array() * lights[i]->I.array()).matrix();

		lights[i]->direction(poi,l,light_src_max_t);
		Ray shadow_ray;
		shadow_ray.origin = poi + epsilon*l;
		shadow_ray.direction = l;
		
/*
		//determine if in a shadow
		if (first_hit(shadow_ray,epsilon,objects,shadow_hit_id,shadow_t,shadow_n)){
			if (shadow_t >= epsilon && shadow_t <= light_src_max_t){ 
				//hit an object on the way to light
				continue;
			}

		}
*/	
		shadow_hit(shadow_ray,epsilon,light_src_max_t,objects,shadow_hit_id,shadow_t,hit_id,shadow_col,shadow_n);	

		h = l + v;
		h.normalize();

		diffuse_col += (scalar_diffuse * max( 0,l.dot(n) ));
		spec_col += (scalar_spec * std::pow(max(0,n.dot(h)), material->phong_exponent));	
	}	


  	rgb = ((ambient_col + diffuse_col + spec_col).array() * shadow_col.array()).matrix();
	return rgb;
}




bool shadow_hit(
  const Ray & ray, 
  const double min_t,
  const double max_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  const int my_id,
  Eigen::Vector3d & shadow_color, //colour of shadow, effected by transparency
  Eigen::Vector3d & n)
{
	hit_id = -1;
	
	Eigen::Vector3d normal; //normal to POI for each object, used for each intersection test
	double t_tmp = 1; //t used in each check
	
	shadow_color = Eigen::Vector3d(1,1,1);

	for (int i = 0; i < objects.size(); i++){

		if (objects[i]->intersect(ray, min_t,t_tmp,normal) && i != my_id){
			if (t_tmp <= max_t && t_tmp >= min_t){
				hit_id = i;
				n = normal;
				t = t_tmp;
		
				shadow_color = (shadow_color.array() * objects[i]->material->kt.array()).matrix();
				
			}	
		}

	}

	if (hit_id == -1){
		shadow_color = Eigen::Vector3d(1,1,1);
		return false;
	}

	return true;

}
