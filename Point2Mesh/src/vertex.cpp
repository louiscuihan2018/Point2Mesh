//
//  vertex.cpp
//  CGL
//
//  Created by louis on 8/4/20.
//

#include <stdio.h>
#include <vector>
#include "vertex.h"
#include "CGL/CGL.h"
#include "CGL/vector3D.h"

using namespace std;
using namespace CGL;
    
Vertex::Vertex(Vector3D a, Vector3D b) {
    this->point = a;
    this->normal = b;
    this->is_inner = false;
    this->adjacent_edges.clear();
    this->adjacent_triangles.clear();
}

void Vertex::add_edge(Edge* edge) {
    this->adjacent_edges.push_back(edge);
}

void Vertex::add_triangle(Triangle* triangle) {
    this->adjacent_triangles.push_back(triangle);
}
    
