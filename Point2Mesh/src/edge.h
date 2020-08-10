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
#include "types.h"

using namespace std;
using namespace CGL;

enum e_type { BORDER = 0, E_FRONT = 1, E_INNER = 2 };
class Edge{
    public:
        Vertex* a;
        Vertex* b;
        Triangle* face1;
        Triangle* face2;
        e_type type;
    // future constructors use pointers
        Edge(){};
        Edge(Vertex* a, Vertex* b);
        bool add_triangle(Triangle *triangle);
        Vertex* from() const;
        Vertex* to() const;
        
        void update_edge();
};
#endif /* edge_h */
