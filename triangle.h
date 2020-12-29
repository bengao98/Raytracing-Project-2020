#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "raytracer.h"

#include "hittable.h"

class triangle : public hittable {
    public:
        triangle() {}

        triangle(point3 _p1, point3 _p2, point3 _p3, shared_ptr<material> m)
            : p1(p1), p2(p2), p3(p3), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 p1;
        point3 p2;
        point3 p3;
        shared_ptr<material> mat_ptr;

};
    bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 origin = r.origin();
    vec3 dir = r.direction();
 
    
    auto p1p2 = p2 - p1; 
    auto p1p3 = p3 - p1; 
    // compute plane's normal
    vec3 N = vec3(1,0,0); // N 
    auto areaT = 0.5*(cross(p1p2, p1p3).length()); 
    

 
    // Step 1: finding intersection
 
    // check if ray and plane are parallel ?
    vec3 w = p1 - origin;
    double a = dot(w, N);
    double b = dot(dir, N); 


 
    // compute t 
    auto t = a / b;
    if (t < t_min || t > t_max) // check if t is in boundary
        return false; 
    
   
 
    // compute the intersection point using equation 1
    vec3 P = origin + t * dir; 
    
    // Barycentric coordinates
    auto a1 = cross(p3 - P, p2 - P);
    auto areaA = 0.5*(a1.length());
    auto b1 = cross(p3 - P, p1 - P);
    auto areaB = 0.5*(b1.length());
    auto c1 = cross(p2 - P, p1 - P);
    auto areaC = 0.5*(c1.length());
    double alpha = areaA / areaT;
    double beta = areaB / areaT;
    double gamma = areaC / areaT;
    double barCoord = alpha + beta + gamma; 

    double kEpsilon = 0.00001;
    if (abs(barCoord - 1) > kEpsilon) // check if barycentric coords = 1
        return false; // ray is not in triangle
   

    rec.t = t;
    rec.p = P;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

 
    return true; // this ray hits the triangle 
}



 #endif