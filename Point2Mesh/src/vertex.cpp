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
#include "edge.h"
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
    Vertex ev1 = *(e.from());
    Vertex ev2 = *(e.to());
    
    Vector3D curr_point = this->point;
    Vector3D a_pos = ev1.point;
    Vector3D b_pos = ev2.point;
    
    Vector3D v1 = a_pos - curr_point;
    Vector3D v2 = b_pos - curr_point;
    
    Vector3D facenormal = cross(v1, v2);
    facenormal.normalize();
    
    Vector3D vn1 = this->normal;
    Vector3D vn2 = ev1.normal;
    Vector3D vn3 = ev2.normal;
    
    bool check_1 = (dot(facenormal, vn1) > 0);
    bool check_2 = (dot(facenormal, vn2) > 0);
    bool check_3 = (dot(facenormal, vn3) > 0);
    
    return check_1 && check_2 && check_3;
    
}

bool Vertex::compatible(Vertex& v1, Vertex& v2) {
    Vector3D a_pos = this->point;
    Vector3D b_pos = v1.point;
    Vector3D c_pos = v2.point;
    
    Vector3D vec1 = b_pos - a_pos;
    Vector3D vec2 = c_pos - a_pos;
    
    Vector3D facenormal = cross(vec1, vec2);
    facenormal.normalize();
    
    Vector3D vn1 = this->normal;
    Vector3D vn2 = v1.normal;
    Vector3D vn3 = v2.normal;
    
    bool check_1 = (dot(facenormal, vn1) > 0);
    bool check_2 = (dot(facenormal, vn2) > 0);
    bool check_3 = (dot(facenormal, vn3) > 0);
       
    return check_1 && check_2 && check_3;
    
}
    
