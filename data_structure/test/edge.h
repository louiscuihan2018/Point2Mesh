//
//  edge.h
//  test
//
//  Created by louis on 8/5/20.
//  Copyright © 2020 louis. All rights reserved.
//

#ifndef edge_h
#define edge_h

#include <vector>
#include "CGL/vector3D.h"
#include "vertex.h"
//#include "triangle.h"

using namespace std;
namespace CGL {
    class Edge{
        public:
            Vertex a;
            Vertex b;
            Tri_list adjacent_triangles;
            bool is_inner;
        // future constructors use pointers
            Edge(){};
            Edge(Vertex a, Vertex b);
    };
}
#endif /* edge_h */
