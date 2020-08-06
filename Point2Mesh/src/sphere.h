//
//  sphere.h
//  ColladaViewer
//
//  Created by louis on 8/4/20.
//

#ifndef sphere_h
#define sphere_h
#include <vector>
#include "CGL/CGL.h"
#include "CGL/vector3D.h"
#include "vertex.h"

using namespace std;
using namespace CGL;

class Sphere{
    public:
        Vector3D center;
        double radius;
        Sphere(){};
        Sphere(CGL::Vector3D v, double r);
    
};
#endif /* sphere_h */
