#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>

#include <stdlib.h>
#include <cmath>
#include <time.h>

/* FEATURE FLAGS */

#define THREAD
#define DOF_ON //Depth of Field 

#include <pthread.h>
#define NUM_THREADS 8

int width =  1920;
int height = 1080;
//int width =  640;
//int height = 360;
std::vector<unsigned char> rgb_image(3*width*height);

struct thread_args{

	int t_id;
	int width;
	int height;
	std::vector<unsigned char> rgb_im;

	//for each threads for loop section
	int start_row;
	int end_row;

	Camera camera;
  	std::vector< std::shared_ptr<Object> > objects;
  	std::vector< std::shared_ptr<Light> >  lights;

};

static void* runner(void * args){
	struct thread_args *my_args = (struct thread_args*)(args);
	
	int start_row = my_args->start_row;
	int end_row = my_args->end_row;
	const Camera & camera = my_args->camera;
  	const std::vector< std::shared_ptr<Object> >  objects = my_args->objects;
  	const std::vector< std::shared_ptr<Light> >  lights = my_args->lights;

	//std::cout << "Started thread: " << my_args->t_id << "\n";	
	//std::cout << "start: " << start_row << "\n";	
	//std::cout << "end: " << end_row << "\n";	

	
	for(unsigned i = start_row; i < end_row; i++){
		for(unsigned j = 0; j < width; j++){
  	    	// Set background color
  	    	Eigen::Vector3d rgb(0,0,0);

  	    	// Compute viewing ray
  	    	Ray ray;
  	    	viewing_ray(camera,i,j,width,height,ray);

#ifdef DOF_ON	    
			const int num_dof_samples = 25;
			//const double lens_size = 0.014;
			const double lens_size = 32;
			const double focal_length = camera.focal_plane;
  	    	Eigen::Vector3d rgb_dof(0,0,0);
			for (int k = 0; k < num_dof_samples; k++){
				//shift ray origin
			
				double rand_num1 = (double) rand() / RAND_MAX;
				double rand_num2 = (double) rand() / RAND_MAX;

				double a = rand_num1 * 2 * M_PI;
				double r = std::sqrt(rand_num2) * (1.0/lens_size);
	
				double rand_u = (-lens_size/2.0) + rand_num1*lens_size;
				double rand_v = (-lens_size/2.0) + rand_num2*lens_size;

				Ray dof_ray;
				dof_ray.origin = ray.origin + r*cos(a)*camera.u + r*sin(a)*camera.v;
				Eigen::Vector3d focus_point;
				focus_point = ray.origin + focal_length*ray.direction;
				dof_ray.direction = focus_point - dof_ray.origin;
 				dof_ray.direction.normalize();	
				
  	    		raycolor(dof_ray,1.0,camera,objects,lights,0,rgb);
				rgb_dof += rgb;

			}
			rgb = rgb_dof * (1.0/num_dof_samples);	
  	    	
#else
			// Shoot ray and collect color
  	    	raycolor(ray,1.0,camera,objects,lights,0,rgb);
#endif
  	    	// Write double precision color into image
  	    	auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
  	    	rgb_image[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
  	    	rgb_image[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
  	    	rgb_image[2+3*(j+width*i)] = 255.0*clamp(rgb(2));
		}
	}
	std::cout << "Thread: " << my_args->t_id << " done.\n";
	pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
	Camera camera;
  	std::vector< std::shared_ptr<Object> > objects;
  	std::vector< std::shared_ptr<Light> > lights;
  	// Read a camera and scene description from given .json file
  	read_json(
  	  argc<=1?"../data/sphere-and-plane.json":argv[1],
  	  camera,
  	  objects,
  	  lights);

	//init threads
	pthread_t threads[NUM_THREADS];
	struct thread_args t_args[NUM_THREADS];	

  	//init random generator
  	srand(time(NULL));
  	

	for(int i = 0; i < NUM_THREADS; i++){
		t_args[i].t_id = i;

		t_args[i].start_row = (height / NUM_THREADS)*i;
		t_args[i].end_row = t_args[i].start_row + (height/NUM_THREADS);

		t_args[i].camera = camera;
		t_args[i].objects = objects;
		t_args[i].lights = lights;

		int error = pthread_create(&threads[i], NULL, runner, (void*)&t_args[i]);
		if (error){
			std::cout << "ERROR: Cannot create thread " << i << "\n";
		}
		
	}

	for(int i = 0; i < NUM_THREADS; i++){
		int error = pthread_join(threads[i], NULL);
	}

  	write_ppm("rgb.ppm",rgb_image,width,height,3);
	return 0;
}

