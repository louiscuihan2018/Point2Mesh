//
//  vertex.cpp
//  CGL
//
//  Created by louis on 8/4/20.
//

#include <stdio.h>
#include <vector>
#include "vertex.h"
#include "edge.h"
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

bool Vertex::compatible(Edge& e) {
    Vector3D fnormal = cross(this->point - e.from()->point, e.to()->point - e.from()->point);
    fnormal.normalize();

    return dot(fnormal, this->normal) > 0 && dot(fnormal, e.from()->normal) > 0 && dot(fnormal, e.to()->normal) > 0;
}

bool Vertex::compatible(Vertex& v1, Vertex& v2) {
    Vector3D& a_pos = this->point;
    Vector3D& b_pos = v1.point;
    Vector3D& c_pos = v2.point;
    
    Vector3D vec1 = b_pos - a_pos;
    Vector3D vec2 = c_pos - a_pos;
    
    Vector3D facenormal = cross(vec1, vec2);
    facenormal.normalize();

    if (dot(facenormal, this->normal) < 0) {
        facenormal = -facenormal;
    }

    return dot(facenormal, v1.normal) > 0 && dot(facenormal, v2.normal) > 0;
}
    
