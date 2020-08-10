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
    
    Edge* bc = v2->edgeTo(*v3);
    if (bc == NULL)
        bc = new Edge(v2, v3);
    
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

bool Triangle::construct_ball(double p, Sphere& s) {
    
    Vector3D a = this->a->point;
    Vector3D b = this->b->point;
    Vector3D c = this->c->point;
    Vector3D ac = c - a;
    Vector3D ab = b - a;
    Vector3D abXac = cross(ab, ac);
    Vector3D acXab = cross(ac, ab);
    Vector3D curr = ac.norm2() * (cross(abXac, ab)) + ab.norm2() * (cross(acXab, ac));
    curr = curr / (2.0 * abXac.norm2());
    Vector3D circumcenter = a + curr;
    
    double dist = curr.norm();
    
//        Vector3D diff_1 = circumcenter - a;
//        double dist_1 = diff.norm();
//        Vector3D diff_2 = circumcenter - b;
//        double dist_2 = diff.norm();
//        Vector3D diff_3 = circumcenter - c;
//        double dist_3 = diff.norm();
    
    if (p < dist) return false;

    double adjustment = sqrt(p * p - dist * dist);
    abXac.normalize();
    Vector3D facenormal = abXac;
    Vector3D center = circumcenter -abXac *adjustment;
    
    s = Sphere(center, p);
    return true;
}

