//
//  vertex.cpp
//  CGL
//
//  Created by louis on 8/4/20.
//

#include <stdio.h>
#include <vector>
#include "vertex.h"
#include "triangle.h"
#include "edge.h"
#include "CGL/CGL.h"
#include "CGL/vector3D.h"

using namespace std;
using namespace CGL;
    
Vertex::Vertex(Vector3D a, Vector3D b) {
    this->point = a;
    this->normal = b;
    this->type = v_type::ORPHAN;
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

Edge* Vertex::edgeTo(Vertex& v) {
    for (auto e : adjacent_edges) {
        if (e->a == &v || e->b == &v) return e;
    }
    return NULL;
}

void Vertex::updateType() {
    if (adjacent_edges.size() == 0) type = v_type::ORPHAN;

    for (auto e : adjacent_edges) {
        if (e->type != 2) {
            type = v_type::FRONT;
            return;
        }
    }

    type = v_type::INNER;
}

Vertex* Vertex::post_Helper(Vertex* v) {
    Edge* e = edgeTo(*v);
    Triangle* t = e->face1;
    for (Edge* curr_e : adjacent_edges) {
        if (curr_e->type != BORDER) {
            continue;
        }
        Vertex* curr_vs = curr_e->from();
        
        if (curr_vs == this) {
            continue;
        }
        if (curr_vs == t->a || curr_vs == t->b || curr_vs == t->c) {
            continue;
        }
        Edge* e_ad = curr_vs->edgeTo(*v);
        
        if (e_ad == NULL) {
            continue;
        }
        
        if (e_ad->type != BORDER) {
            continue;
        }
        if (e_ad->from() == v) {
            return v;
        }
    }
    return NULL;
}
    
