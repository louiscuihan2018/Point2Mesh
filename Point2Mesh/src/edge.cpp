//
//  edge.cpp
//  test
//
//  Created by louis on 8/5/20.
//  Copyright © 2020 louis. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "vertex.h"
#include "edge.h"
#include "triangle.h"
#include <vector>
#include "CGL/CGL.h"
#include "CGL/vector3D.h"

using namespace std;

Edge::Edge(Vertex* a, Vertex* b) {
    this->a = a;
    this->b = b;
    this->type = e_type::E_FRONT;
    this->a->add_edge(this);
    this->b->add_edge(this);
    this->face1 = NULL;
    this->face2 = NULL;
}
bool Edge::add_triangle(Triangle* triangle) {
    if (this->face1 == triangle || this->face2 == triangle) return false;

    if (this->face1 == NULL) {
        this->face1 = triangle;
        this->type = e_type::E_FRONT;
        update_edge();
    }
    else if (this->face2 == NULL) {
        this->face2 = triangle;
        this->type = e_type::E_INNER;
    }
    else {
        cout<< "Already two triangles" <<endl;
        return false;
    }
    return true;
}

Vertex* Edge::from() const {
    return this->a;
}

Vertex* Edge::to() const {
    return this->b;
}

void Edge::update_edge() {
    // step1: find the opposite vertex to this edge
    Vertex* opp = NULL;
    if (face1->a != this->a && face1->a != this->b) opp = face1->a;
    else if (face1->b != this->a && face1->b != this->b) opp = face1->b;
    else if (face1->c != this->a && face1->c != this->b) opp = face1->c;
    // step2: switch a & b if orientation is wrong...
    if (opp == NULL) return;
    Vector3D fnorm = cross(this->b->point - this->a->point, opp->point - this->a->point);
    fnorm.normalize();

    Vector3D vnorm = this->a->point + this->b->point + opp->point;
    vnorm.normalize();

    if (dot(fnorm, vnorm) < 0) {
        // use opp as tmp here, don't worry
        opp = this->a;
        this->a = this->b;
        this->b = opp;
    }
}
