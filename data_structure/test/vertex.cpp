//
//  vertex.cpp
//  CGL
//
//  Created by louis on 8/4/20.
//

#include <stdio.h>
#include "vertex.h"
#include <vector>
#include "CGL/vector3D.h"

using namespace std;

namespace CGL {
    
    Vertex::Vertex(Vector3D a, Vector3D b) {
        this->point = a;
        this->normal = b;
        this->is_inner = false;
//        vector<Vertex> ps;
//        this->neighbors = ps;
    }
    
//    void Vertex::add_edge(Edge* edge) {
//        this->adjacent_edges.insert(edge);
//    }

}
