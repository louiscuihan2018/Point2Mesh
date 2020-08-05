//
//  triangle.cpp
//  CGL
//
//  Created by louis on 8/4/20.
//

#include <stdio.h>
#include <vector>
#include <math.h>
#include "CGL/vector3D.h"
#include "triangle.h"
#include "edge.h"
#include "sphere.h"


using namespace std;

namespace CGL {

    Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3) {
        this->a = v1;
        this->b = v2;
        this->c = v3;
        Edge ab = Edge(v1, v2);
        Edge bc = Edge(v2, v3);
        Edge ca = Edge(v3, v1);
        this->ab = ab;
        this->bc = bc;
        this->ca = ca;
    }

    Sphere Triangle::construct_ball(double p) {
        
        Vector3D a = this->a.point;
        Vector3D b = this->b.point;
        Vector3D c = this->c.point;
        Vector3D ac = c - a;
        Vector3D ab = b - a;
        Vector3D abXac = cross(ab, ac);
        Vector3D acXab = cross(ac, ab);
        Vector3D curr = ac.norm2() * (cross(abXac, ab)) + ab.norm2() * (cross(acXab, ac));
        curr = curr / (2.0 * abXac.norm2());
        Vector3D circumcenter = a + curr;
        
        Vector3D diff = circumcenter - a;
        double dist = diff.norm();
        
//        Vector3D diff_1 = circumcenter - a;
//        double dist_1 = diff.norm();
//        Vector3D diff_2 = circumcenter - b;
//        double dist_2 = diff.norm();
//        Vector3D diff_3 = circumcenter - c;
//        double dist_3 = diff.norm();
        
        double adjustment = sqrt(p * p - dist * dist);
        abXac.normalize();
        Vector3D facenormal = abXac;
        Vector3D center = circumcenter -abXac *adjustment;
        
        Sphere s = Sphere(center, p);
        return s;
        
    }

}
