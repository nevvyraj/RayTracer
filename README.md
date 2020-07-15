LEC2201

Image: "Spheres" 

For my final project I built on Assignment 3 to add more features. The features I added are:

1. Spotlight light source: include/SpotLight.h, src/SpotLight.cpp
    - the idea was to build on point light but include an angle so that the light has a cone shape
    - used on the purple ball in image
    
2. Refraction/Transparency/Fresnel: include/refract.h, src/refract.cpp, src/raycolor.cpp (is_transmissive and is_dielectric if statements)
    - The fresnel implementation was based on the Schlick approximation outlined in chapter 13 of the course textbook
    - used on glass ball on the left
    
3. Multithread support: main.cpp
    - used pthreads to multithread the ray tracer so each thread works on a separate part of the image to speed it up
    
3. Depth of field: main.cpp
    - used math in https://programming.guide/random-point-within-circle.html to generate random numbers in a uniform circle to create the sample rays to shoot
	- shown in spheres at the back
    
4. Glossy reflection: src/raycolor.cpp (is_glossy if statement)
    - used chapter 13 of course textbook as a guide
    - used on teal metallic ball on the right of the image
    
5. Translucent Shadows: function shadow_hit in src/blinn_phong_shading.cpp
    - used so that transulcent/transparent objects don't have opaque shadows
    - essentially multiplying pixel color of the point by the "kt" transmissive color of each object on the path to the light, effectively darkening color if transulucent object in path or completely black hard shadow if opqaue object in the way
    
    
cmake command to build
> cmake .. -DTHREAD=ON -DCMAKE_BUILD_TYPE=Release

Output image is 1920x1080 ppm file. Used https://convertio.co to convert to PNG

