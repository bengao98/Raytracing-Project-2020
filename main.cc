#include "raytracer.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "cube.h"
#include "light.h"

#include <iostream>


color ray_color(const ray& r, const hittable& world, int depth, bool type, directLight dl, pointLight pl) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        hit_record newRec; 
        ray scattered;
        ray shadow;
        if (type == 1)
            shadow = ray(rec.p, dl.direction);
         else 
            shadow = ray(rec.p, pl.origin - rec.p);
        color attenuation;
        color matColor; 
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered, type, dl, pl, matColor)) {
            if (world.hit(shadow, 0.001, infinity, newRec))
                return attenuation / 8 * ray_color(scattered, world, depth-1, type, dl, pl);
            return matColor * ray_color(scattered, world, depth-1, type, dl, pl);
        }
        return color(0,0,0);
    }



    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) { // to scatter spheres
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<glass>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<glass>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


hittable_list cube_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    auto material1 = make_shared<lambertian>(color(0.4, 0.2, 0.1));

    world.add(make_shared<triangle>(point3(0,-1,0), point3(2,1,0), point3(-2,1,2), material1));
    world.add(make_shared<cube>(point3(0, 1, 3), 4.0, material1));

   

    return world;
}

hittable_list shadow_scene(){
    hittable_list world;
    
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    auto material1 = make_shared<lambertian>(color(0.4, 0.2, 0.1));

    auto material2 = make_shared<blinn>(color(0.4, 0.2, 0.1), color(0.6, 0.5, 0.4));
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    world.add(make_shared<sphere>(point3(0, 1, 3), 1.0, material1));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(0, 1, -3), 1.0, material3));   

    return world;
}


int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 2;

    // World

    auto world = shadow_scene();

    directLight dl = directLight(color(1.0, 1.0, 1.0), unit_vector(vec3(1.0, 0.5, 5.0)));
    pointLight pl = pointLight(color(1.0, 1.0, 1.0), vec3(5.0, 5.0, 5.0));
    bool type = 0; // if 1 = directLight, if 0 = pointLight
    


    // Camera

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 upv(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, upv, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth, type, dl, pl);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}


