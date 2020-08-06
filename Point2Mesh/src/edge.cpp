//
//  edge.cpp
//  test
//
//  Created by louis on 8/5/20.
//  Copyright © 2020 louis. All rights reserved.
//

#include <stdio.h>
#include <iostream>
//#include "vertex.h"
#include "edge.h"
#include <vector>
#include "CGL/CGL.h"
#include "CGL/vector3D.h"

using namespace std;

Edge::Edge(Vertex* a, Vertex* b) {
    this->a = a;
    this->b = b;
    this->is_inner = false;
    this->a->add_edge(this);
    this->b->add_edge(this);
    this->face1 = NULL;
    this->face2 = NULL;
}
void Edge::add_triangle(Triangle* triangle) {
    if (this->face1 == NULL) {
        this->face1 = triangle;
    }
    else if (this->face2 == NULL) {
        this->face2 = triangle;
    }
    else {
        cout<< "Already two triangles" <<endl;
    }
}

Vertex* Edge::geta() {
    return this->a;
}

Vertex* Edge::getb() {
    return this->b;
}

