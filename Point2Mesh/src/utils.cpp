//
//  utils.cpp
//  CGL
//
//  Created by louis on 8/5/20.
//

#include <stdio.h>
#include "utils.h"
#include <iostream>
#include "vertex.h"
#include "edge.h"
#include "sphere.h"
#include "triangle.h"
using namespace CGL;

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c) {
    Vertex v1 = *a;
    Vertex v2 = *b;
    Vertex v3 = *c;
    
    bool check = v1.compatible(v2, v3);
    if (check) {
        Triangle* t = new Triangle(a, b, c);
        return std::make_pair(t, true);
    } else {
        Triangle* t = NULL;
        return std::make_pair(t, true);
    }
}
