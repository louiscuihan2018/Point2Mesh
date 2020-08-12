//
//  triangle.cpp
//  CGL
//
//  Created by louis on 8/4/20.
//

#include <stdio.h>
#include <vector>
#include <math.h>
#include "CGL/CGL.h"
#include "CGL/vector3D.h"
#include "triangle.h"
#include "edge.h"
#include "sphere.h"


using namespace std;
using namespace CGL;

Triangle::Triangle(Vertex* v1, Vertex* v2, Vertex* v3) {
    this->a = v1;
    this->b = v2;
    this->c = v3;
    
    Edge* ab = v1->edgeTo(*v2);
    if (ab == NULL)
        ab = new Edge(v1, v2);
    this->ab = ab;
    Edge* bc = v2->edgeTo(*v3);
    if (bc == NULL)
        bc = new Edge(v2, v3);
    this->bc = bc;
    Edge* ca = v3->edgeTo(*v1);
    if (ca == NULL) 
        ca = new Edge(v3, v1);

    this->ab->add_triangle(this);
    this->bc->add_triangle(this);
    this->ca->add_triangle(this);

    this->a->add_triangle(this);
    this->b->add_triangle(this);
    this->c->add_triangle(this);
    this->a->updateType();
    this->b->updateType();
    this->c->updateType();
}

