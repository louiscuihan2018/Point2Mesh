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

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c);

pair< vector<Vector3D>, vector<Vertex> > read_and_range(string name);

Triangle* FindSeedTriangle(OcTree* tree, double r);

Triangle* FindSeedTriangle(OcTree* tree, OcNode* node, double r);

Triangle* FindSeedTriangle(vector<Vertex*> vlist, double r);

void demo_read();

#endif /* utils_hj_h */
