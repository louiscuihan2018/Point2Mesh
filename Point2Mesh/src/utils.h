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
uint read_and_range(string name, pair< vector<Vector3D>&, vector<Vertex>& >* data);

Triangle* FindSeedTriangle(OcTree* tree, double r);

Triangle* FindSeedTriangle(OcTree* tree, OcNode* node, double r);

Triangle* FindSeedTriangle(vector<Vertex*> vlist, double r);

#endif /* utils_hj_h */