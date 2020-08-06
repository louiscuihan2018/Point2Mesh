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
using namespace CGL;

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c);

vector<Vector3D> read_and_range();


#endif /* utils_hj_h */
