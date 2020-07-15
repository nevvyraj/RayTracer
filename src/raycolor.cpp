#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include "refract.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

#define PRINT_VEC(d) (std::cout << #d << ": " << d(0) << ", " << d(1) << ", " << d(2) << "\n")

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const Camera & camera, //used to get u and v vectors 
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{

	
	//first hit args
	int hit_id;
	double t;
	Eigen::Vector3d normal;


	Eigen::Vector3d poi; //intersection point	

	const int max_depth = 4;
	double epsilon = 1e-6;
	//std::cout << "Call: " << num_recursive_calls << "\n";	

	if (first_hit(ray,min_t,objects,hit_id,t,normal)){
		poi = ray.origin + t*ray.direction;
		
		//shading model	
		rgb = blinn_phong_shading(ray,hit_id,t,normal,objects,lights);

		bool is_metallic = (objects[hit_id]->material->km != Eigen::Vector3d::Zero() && objects[hit_id]->material->kt == Eigen::Vector3d::Zero());
		bool is_dialectric = (objects[hit_id]->material->km != Eigen::Vector3d::Zero() && objects[hit_id]->material->kt != Eigen::Vector3d::Zero());
		bool is_transmissive = (objects[hit_id]->material->km == Eigen::Vector3d::Zero() && objects[hit_id]->material->kt != Eigen::Vector3d::Zero());
		bool is_glossy = objects[hit_id]->material->is_glossy && objects[hit_id]->material->km != Eigen::Vector3d::Zero();

		if (num_recursive_calls < max_depth){
		
			
			//if (objects[hit_id]->material->km != Eigen::Vector3d::Zero() && raycolor(reflected_ray,epsilon,objects,lights,num_recursive_calls + 1, reflected_rgb)) 
			if (is_metallic)
			{
				//reflection
				Eigen::Vector3d reflected_rgb;
				
				Ray reflected_ray;
		   		reflected_ray.direction	= reflect(ray.direction, normal);
				reflected_ray.origin = poi;
		

				if (is_glossy){ //glossy reflection	
					double rand_num1 = 0.0;	
					double rand_num2 = 0.0;	
					double rand_u, rand_v;
					const double blurr_degree = 0.55;
					Eigen::Vector3d gloss_rgb(0,0,0);
					
					const int num_gloss_samples = 25;				
					for (int i = 0; i < num_gloss_samples; i++){
						rand_num1 = (double)rand() / RAND_MAX;		
						rand_num2 = (double)rand() / RAND_MAX;		
			
						rand_u = (-blurr_degree/2.0) + rand_num1*blurr_degree;
						rand_v = (-blurr_degree/2.0) + rand_num2*blurr_degree;
	
						Ray shifted_rray; //shifted reflection ray
						shifted_rray.direction = reflected_ray.direction + rand_u*camera.u + rand_v*camera.v;
						shifted_rray.direction.normalize();			
						shifted_rray.origin = poi + epsilon*shifted_rray.direction;
	
						if (raycolor(shifted_rray,epsilon,camera,objects,lights,num_recursive_calls + 1, reflected_rgb)) 
						{
							gloss_rgb += ( reflected_rgb.array() * (objects[hit_id]->material->km.array())).matrix();	
						}

						//rgb += (gloss_rgb);
					}	
					rgb += (gloss_rgb) * 1.0/num_gloss_samples;
				} //glossy

				//specular reflection
				else{
						if (raycolor(reflected_ray,epsilon,camera,objects,lights,num_recursive_calls + 1, reflected_rgb)) 
						{
							rgb += ( reflected_rgb.array() * (objects[hit_id]->material->km.array())).matrix();	
						}
				}

			}	

			//refraction
			//if (objects[hit_id]->material->kt != Eigen::Vector3d::Zero())
			else if (is_transmissive)
			{
				Ray refracted_ray;
				refracted_ray.origin = poi;
				refracted_ray.direction = Eigen::Vector3d::Zero();
				
				double c1,c2;
				if (ray.direction.normalized().dot(normal) < 0.0)
				{ //going into object
					c1 = 1.0;
					c2 = objects[hit_id]->material->ior;
				}
				else
				{ //coming out of object
					c1 = objects[hit_id]->material->ior;
					c2 = 1.0;
					normal = -normal;
				}
				if (refract(ray.direction,normal,refracted_ray.direction,c1,c2))
				{

					Eigen::Vector3d refracted_rgb;
					if(raycolor(refracted_ray,epsilon,camera,objects,lights,num_recursive_calls + 1, refracted_rgb))
					{
						rgb += ( refracted_rgb.array() * (objects[hit_id]->material->kt.array())).matrix();	
					}
	
				}
				else{
					//std::cout << "TOTAL INTERNAL REFLECTION\n";
				}
			} //refraction


			//Applying Schlick Approximation of Fresnel
			else if (is_dialectric){
			//if (is_dialectric && 0){
				Ray reflected_ray;
		   		reflected_ray.direction	= reflect(ray.direction, normal);
				reflected_ray.origin = poi;

				Ray refracted_ray;
				refracted_ray.origin = poi;
				
				double cosine = 0.0; //for fresnel
				cosine = ray.direction.normalized().dot(normal);
				
				double c1,c2;	
				double ior = objects[hit_id]->material->ior;
				if (cosine < 0.0)
				{ //going into object
					c1 = 1.0;
					c2 = ior;
					cosine = -ray.direction.normalized().dot(normal); 
					refract(ray.direction,normal,refracted_ray.direction,c1,c2);
				} else {
					c1 = ior;
					c2 = 1.0;
					if (refract(ray.direction,-normal,refracted_ray.direction,c1,c2)){
						cosine = refracted_ray.direction.normalized().dot(normal);
						//cosine = refracted_ray.direction.dot(normal);
					}
					else{ //total internal reflection
						Eigen::Vector3d reflected_rgb;
						if (raycolor(reflected_ray,epsilon,camera,objects,lights,num_recursive_calls + 1, reflected_rgb)) 
						{
							rgb += ( reflected_rgb.array() * (objects[hit_id]->material->km.array())).matrix();
							return true;	
						}
					}
					
				}

				double r0 = ((ior-1.0)*(ior-1.0)) / ((ior+1.0)*(ior+1.0));
				double cos = (1-cosine);
				cos = cos*cos*cos*cos*cos;
				double r = r0 + (1-r0)*cos; //fresnel

				Eigen::Vector3d reflected_rgb;
				if (raycolor(reflected_ray,epsilon,camera,objects,lights,num_recursive_calls + 1, reflected_rgb)) 
				{
					rgb += r*( reflected_rgb.array() * (objects[hit_id]->material->km.array())).matrix();
				}
				Eigen::Vector3d refracted_rgb;
				if(raycolor(refracted_ray,epsilon,camera,objects,lights,num_recursive_calls + 1, refracted_rgb))
				{
					rgb += (1-r)*( refracted_rgb.array() * (objects[hit_id]->material->kt.array())).matrix();	
				}			
			} //dialectric

		} //recursive calls

		return true;
	} //if ray hit object	

	else{
		rgb << 0,0,0; //return black background color
	}

  return false;
}




