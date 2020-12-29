#ifndef LIGHT_H
#define LIGHT_H

#include "raytracer.h"
#include "color.h"

class light {
    public: light(const color& l) : lightColor(l) {}
            
                
    public: color lightColor;



};



class pointLight : public light {
    public: pointLight(const color& l, vec3 pos) : light(l), origin(pos) {}
        
    public: vec3 origin; 



};


class directLight : public light {
    public: directLight(const color& l, vec3 dir) : light(l), direction(dir) {}
            
    public: vec3 direction; 

};

#endif