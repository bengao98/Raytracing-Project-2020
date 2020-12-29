#ifndef CUBE_H
#define CUBE_H
#include "raytracer.h"

#include "hittable_list.h"
#include "triangle.h"

#include <memory>
#include <vector>


class cube : public hittable  {
    public:
        cube() {}
        cube(point3 p0, double length, shared_ptr<material> ptr);

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 cube_min;
        double len;
        hittable_list sides;
        
};

cube::cube(point3 p0, double length, shared_ptr<material> ptr) {
    cube_min = p0;
    len = length;
   

    sides.add(make_shared<triangle>(point3(p0.x(), p0.y() + len, p0.z()), point3(p0.x(), p0.y(), p0.z()), point3(p0.x(), p0.y(), p0.z()  + len), ptr));
    sides.add(make_shared<triangle>(point3(p0.x(), p0.y() + len, p0.z()), point3(p0.x() + len, p0.y() + len, p0.z() + len), point3(p0.x(), p0.y(), p0.z()  + len), ptr));  

    sides.add(make_shared<triangle>(point3(p0.x()  + len, p0.y(), p0.z()), point3(p0.x()  + len, p0.y(), p0.z()  + len), point3(p0.x(), p0.y(), p0.z()  + len), ptr)); 
    sides.add(make_shared<triangle>(point3(p0.x()  + len, p0.y(), p0.z()  + len), point3(p0.x(), p0.y()  + len, p0.z()  + len), point3(p0.x(), p0.y(), p0.z()  + len), ptr)); 

    sides.add(make_shared<triangle>(point3(p0.x(), p0.y()  + len, p0.z()  + len), point3(p0.x()  + len, p0.y()  + len, p0.z()  + len), point3(p0.x()  + len, p0.y()  + len, p0.z()), ptr)); 
    sides.add(make_shared<triangle>(point3(p0.x(), p0.y() + len, p0.z() + len), point3(p0.x(), p0.y() + len, p0.z()), point3(p0.x() + len, p0.y() + len, p0.z()), ptr)); 

    sides.add(make_shared<triangle>(point3(p0.x() + len, p0.y(), p0.z() + len), point3(p0.x() + len, p0.y() + len, p0.z() + len), point3(p0.x() + len, p0.y() + len, p0.z()), ptr)); 
    sides.add(make_shared<triangle>(point3(p0.x() + len, p0.y(), p0.z() + len), point3(p0.x() + len, p0.y(), p0.z()), point3(p0.x() + len, p0.y() + len, p0.z()), ptr)); 

    sides.add(make_shared<triangle>(point3(p0.x(), p0.y(), p0.z()), point3(p0.x(), p0.y() + len, p0.z()), point3(p0.x() + len, p0.y() + len, p0.z()), ptr));
    sides.add(make_shared<triangle>(point3(p0.x(), p0.y(), p0.z()), point3(p0.x() + len, p0.y(), p0.z()), point3(p0.x() + len, p0.y() + len, p0.z()), ptr));

    sides.add(make_shared<triangle>(point3(p0.x(), p0.y(), p0.z() + len), point3(p0.x(), p0.y() + len, p0.z() + len), point3(p0.x() + len, p0.y() + len, p0.z() + len), ptr));
    sides.add(make_shared<triangle>(point3(p0.x(), p0.y(), p0.z() + len), point3(p0.x() + len, p0.y(), p0.z() + len), point3(p0.x() + len, p0.y() + len, p0.z() + len), ptr));

}


bool cube::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
   return sides.hit(r, t_min, t_max, rec);
}


#endif