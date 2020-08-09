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
#include "CGL/CGL.h"
#include "CGL/vector3D.h"
#include "vertex.h"
//#include "triangle.h"

using namespace std;

class Edge{
    public:
        Vertex* a;
        Vertex* b;
        Triangle* face1;
        Triangle* face2;
        bool is_inner;
    // future constructors use pointers
        Edge(){};
        Edge(Vertex* a, Vertex* b);
        bool add_triangle(Triangle *triangle);
        Vertex* from() const;
        Vertex* to() const;
        
};
#endif /* edge_h */
