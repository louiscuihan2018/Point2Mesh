//
//  edge.cpp
//  test
//
//  Created by louis on 8/5/20.
//  Copyright © 2020 louis. All rights reserved.
//

#include <stdio.h>
#include "vertex.h"
#include "edge.h"
#include <vector>
#include "CGL/vector3D.h"

using namespace std;

namespace CGL {
    
    Edge::Edge(Vertex* a, Vertex* b) {
        this->a = a;
        this->b = b;
        this->is_inner = false;
//        this->a.adjacent_edges.push_back(&this);
//        this->b.adjacent_edges.push_back(&this);
    }

}
