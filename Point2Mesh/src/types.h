//
//  types.h
//  test
//
//  Created by louis on 8/5/20.
//  Copyright © 2020 louis. All rights reserved.
//
//#define uint unsigned int
#ifndef types_h
#define types_h
#include <map>

class Vertex;
class Edge;
class Triangle;

typedef unsigned int uint;
typedef std::vector<Edge*> Edge_list;
typedef std::vector<Triangle*> Tri_list;
typedef std::multimap<double, Vertex*> Neighbor_map;
typedef std::multimap<double, Vertex*>::iterator Neighbor_iter;

#endif /* types_h */
