//
//  triangle.h
//  ColladaViewer
//
//  Created by louis on 8/4/20.
//

#ifndef triangle_h
#define triangle_h
#include <vector>
#include "CGL/CGL.h"
#include "CGL/vector3D.h"
#include "vertex.h"
#include "edge.h"
#include "sphere.h"


using namespace std;
using namespace CGL;

class Triangle{
    public:
        Vertex* a;
        Vertex* b;
        Vertex* c;
        Edge* ab;
        Edge* bc;
        Edge* ca;
        Triangle(){};
        Triangle(Vertex* v1, Vertex* v2, Vertex* v3);
        Sphere construct_ball(double p);
};
#endif /* triangle_h */
