//
//  utils.h
//  Point2Mesh
//
//  Created by louis on 8/5/20.
//

#ifndef utils_h
#define utils_h
#include <iostream>
#include "vertex.h"
#include "edge.h"
#include "sphere.h"
#include "triangle.h"
#include "octree.h"
using namespace CGL;


typedef std::chrono::high_resolution_clock Clock;

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c);

/**
* @param name -> name of the .xyz data file
* @param data -> the container which will hold the data read from method
* @return number of data points
*/
uint read_and_range(string name, pair< vector<Vector3D>&, vector<Vertex*>& >* data);

Triangle* FindSeedTriangle(OcTree* tree, double r);

Triangle* FindSeedTriangle(OcTree* tree, OcNode* node, double r);

Triangle* FindSeedTriangle(vector<Vertex*> vlist, double r);

Vertex* FindCandidate(Edge* e, OcTree* tree, double r);

bool construct_ball(Vertex* v1, Vertex* v2, Vertex* v3, double p, Sphere& s) {
    Vector3D& a = v1->point;
    Vector3D& b = v2->point;
    Vector3D& c = v3->point;
    Vector3D ac = c - a;
    Vector3D ab = b - a;
    Vector3D abXac = cross(ab, ac);
    Vector3D acXab = cross(ac, ab);
    Vector3D curr = ac.norm2() * (cross(abXac, ab)) + ab.norm2() * (cross(acXab, ac));
    curr = curr / (2.0 * abXac.norm2());
    Vector3D circumcenter = a + curr;

    double dist = curr.norm();

    if (p < dist) return false;

    double adjustment = sqrt(p * p - dist * dist);
    abXac.normalize();
    Vector3D facenormal = abXac;
    Vector3D center = circumcenter - abXac * adjustment;

    s = Sphere(center, p);
    return true;
}

#endif /* utils_hj_h */
